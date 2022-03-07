OUTPUT="$PDK_KEYWR_COMP_PATH/binary/${SOC}/test_images"

mkdir -p $OUTPUT

if [ ! -d $PDK_KEYWR_COMP_PATH/scripts/keys ]; then
	cd $PDK_KEYWR_COMP_PATH/scripts
	./gen_keywr_cert.sh -g
fi

echo "Copy known dummy keys to be used for the test cases"
cp $PDK_INSTALL_PATH/ti/build/makerules/k3_dev_mpk.pem $PDK_INSTALL_PATH/ti/boot/keywriter/scripts/keys/smpk.pem
xxd -p -r $PDK_INSTALL_PATH/ti/build/makerules/k3_dev_mek.txt > $PDK_INSTALL_PATH/ti/boot/keywriter/scripts/keys/smek.key

if [ ! -f $PDK_KEYWR_COMP_PATH/scripts/ti_fek_public.pem ]; then
	echo "ERR: ti_fek_public.pem doesn't exist"
	exit
fi

declare -A test1 test2 test3
test1[comment]="Test 1 - multi-stage conversion"
test1[1]="./gen_keywr_cert.sh --msv 0xC0FFE --msv-wp -t ti_fek_public.pem"
test1[2]="./gen_keywr_cert.sh --msv 0xC0FFE --msv-ovrd -t ti_fek_public.pem"
test1[3]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-sbl 3 --sr-sysfw 1 "
test1[4]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-bcfg 5"
test1[5]="./construct_ext_otp_data.sh -extotp 0x80000001 -indx 0 -size 32;
        ./gen_keywr_cert.sh -t ti_fek_public.pem --ext-otp ext_otp_data.bin --ext-otp-indx 0 --ext-otp-size 32 "
test1[6]="./construct_ext_otp_data.sh -extotp 0x81 -indx 32 -size 8;
        ./gen_keywr_cert.sh -t ti_fek_public.pem --ext-otp ext_otp_data.bin --ext-otp-indx 32 --ext-otp-size 8 "
test1[7]="./gen_keywr_cert.sh -t ti_fek_public.pem -b keys/bmpk.pem --bmek keys/bmek.key -b-wp --bmek-wp --mek-opt 0x1 --mpk-opt 0x21"
test1[8]="./gen_keywr_cert.sh -t ti_fek_public.pem -s keys/smpk.pem --smek keys/smek.key -s-wp --smek-wp --mek-opt 0x1 --mpk-opt 0x21"
test1[9]="./gen_keywr_cert.sh -t ti_fek_public.pem --keycnt 2 --keycnt-wp "
test1[10]="./gen_keywr_cert.sh -t ti_fek_public.pem --keyrev 1 "

test2[comment]="Test 2 - single-shot converstion"
test2[1]="./gen_keywr_cert.sh -t ti_fek_public.pem
            -s keys/smpk.pem --smek keys/smek.key -s-wp --smek-wp
            -b keys/bmpk.pem --bmek keys/bmek.key -b-wp --bmek-wp
            --mek-opt 0x1 --mpk-opt 0x21
            --keycnt 2 --keycnt-wp
            --keyrev 1" 

tests=(test1 test2)

echo "make keywriter_img (initial)"
make -C $PDK_INSTALL_PATH/ti/build keywriter_img

function gen_tests()
{
    local -n test_array=$1
    echo "Generating test seqeuence $2"

    # Create a unique test directory for storing the test binaries
    TEST_OUT_DIR=$OUTPUT/test$2
    mkdir -p $TEST_OUT_DIR

    # Dump the test description to into this directory
    echo "${test_array[comment]}" > $TEST_OUT_DIR/test_description.txt

    for i in $(seq 1 ${#test_array[@]})
    do
        i2=$(printf "%02d" $i)
        cd $PDK_KEYWR_COMP_PATH/scripts
        case "$i" in
            "${#test_array[@]}")
                # Don't generate the last iteration since there is a comment key
                continue
                ;;
            *)
                echo "${test_array[$i]}" |& tee -a $TEST_OUT_DIR/test_description.txt
                eval ${test_array[$i]} &> $TEST_OUT_DIR/kw_test${i2}.log
                ;;
        esac

        echo "# Generating test$2 Binary No. ${i}"
        cp $PDK_KEYWR_COMP_PATH/x509cert/final_certificate.bin $TEST_OUT_DIR/cert-kw_test${i2}.bin
        cd $PDK_INSTALL_PATH/ti/build
        echo " make keywriter_img with TC ${i2}"
        make keywriter_img &>> $TEST_OUT_DIR/kw_test${i2}.log
        cp $PDK_KEYWR_COMP_PATH/binary/${SOC}/keywriter_img_${SOC}_release.tiimage $TEST_OUT_DIR/kw_test${i2}.tiimage
        cp $PDK_KEYWR_COMP_PATH/binary/${SOC}/keywriter_img_${SOC}_release.xer5f  $TEST_OUT_DIR/kw_test${i2}.xer5f

        echo "# Done!"
    done
}

for i in $(seq 1 ${#tests[@]}); do
    gen_tests ${tests[$((i-1))]} $i
done
