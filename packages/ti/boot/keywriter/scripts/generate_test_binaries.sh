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

declare -A commands
commands[1]="./gen_keywr_cert.sh --msv 0xC0FFE -t ti_fek_public.pem"
commands[2]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-sbl 3 --sr-sysfw 1 "
commands[3]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-bcfg 5"
commands[4,1]="./construct_ext_otp_data.sh -extotp 0x80000001 -indx 0 -size 32"
commands[4,2]="./gen_keywr_cert.sh -t ti_fek_public.pem --ext-otp ext_otp_data.bin --ext-otp-indx 0 --ext-otp-size 32 "
commands[5,1]="./construct_ext_otp_data.sh -extotp 0x81 -indx 32 -size 8 " 
commands[5,2]="./gen_keywr_cert.sh -t ti_fek_public.pem --ext-otp ext_otp_data.bin --ext-otp-indx 32 --ext-otp-size 8 "
commands[6]="./gen_keywr_cert.sh -t ti_fek_public.pem -b keys/bmpk.pem --bmek keys/bmek.key -b-wp --bmek-wp --mek-opt 0x1 --mpk-opt 0x21"
commands[7]="./gen_keywr_cert.sh -t ti_fek_public.pem -s keys/smpk.pem --smek keys/smek.key -s-wp --smek-wp --mek-opt 0x1 --mpk-opt 0x21"
commands[8]="./gen_keywr_cert.sh -t ti_fek_public.pem --keycnt 2 --keycnt-wp "
commands[9]="./gen_keywr_cert.sh -t ti_fek_public.pem --keyrev 1 "


commands[10]="/gen_keywr_cert.sh -t ti_fek_public.pem --msv 0x0001 "
commands[11]="/gen_keywr_cert.sh -t ti_fek_public.pem --msv 0x0000 --msv-ovrd "
commands[12]="/gen_keywr_cert.sh -t ti_fek_public.pem --msv 0x000F --msv-ovrd "
commands[13]="/gen_keywr_cert.sh -t ti_fek_public.pem --msv 0x00FF --msv-ovrd --msv-wp "
commands[14]="/gen_keywr_cert.sh -t ti_fek_public.pem --msv 0x0FFF --msv-ovrd "

for i in $(seq 1 14)
do
	echo "# Generating Binary No. ${i}"
	cd $PDK_KEYWR_COMP_PATH/scripts
	case "$i" in
		"4"|"5")
			echo " ${commands[$i,1]}"
			${commands[$i,1]} &> $OUTPUT/kw_test${i}.log
			echo " ${commands[$i,2]}"
			${commands[$i,2]} &>> $OUTPUT/kw_test${i}.log
			;;
		*)
			echo " ${commands[$i]}"
			${commands[$i]} &> $OUTPUT/kw_test${i}.log
			;;
	esac

	cp $PDK_KEYWR_COMP_PATH/x509cert/final_certificate.bin $OUTPUT/cert-kw_test${i}.bin
	cd $PDK_INSTALL_PATH/ti/build
	echo " make keywriter_img with TC ${i}"
	make keywriter_img &>> $OUTPUT/kw_test${i}.log
	cp $PDK_KEYWR_COMP_PATH/binary/${SOC}/keywriter_img_${SOC}_release.tiimage $OUTPUT/kw_test${i}.tiimage
	cp $PDK_KEYWR_COMP_PATH/binary/${SOC}/keywriter_img_${SOC}_release.xer5f $OUTPUT/kw_test${i}.xer5f

	echo "# Done!"
done
