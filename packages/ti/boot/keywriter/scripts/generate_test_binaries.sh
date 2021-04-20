PDK="/ti/jacinto/workarea/pdk/packages/ti"
OUTPUT="/tmp/keywriter_binaries"

mkdir -p $OUTPUT

if [ ! -d $PDK/boot/keywriter/scripts/keys ]; then
	cd $PDK/boot/keywriter/scripts
	./gen_keywr_cert.sh -g
fi

if [ ! -f $PDK/boot/keywriter/scripts/ti_fek_public.pem ]; then
	echo "ERR: ti_fek_public.pem doesn't exist"
	exit
fi

declare -A commands
commands[1]="./gen_keywr_cert.sh --msv 0xC0FFE -t ti_fek_public.pem"
commands[2]="./gen_keywr_cert.sh -t ti_fek_public.pem --sr-sbl 3 --sr-sysfw 4 "
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
	cd $PDK/boot/keywriter/scripts
	case "$i" in
		"4"|"5")
			echo " ${commands[$i,1]}"
			${commands[$i,1]} &> $OUTPUT/kw${i}.log
			echo " ${commands[$i,2]}"
			${commands[$i,2]} &>> $OUTPUT/kw${i}.log
			;;
		*)
			echo " ${commands[$i]}"
			${commands[$i]} &> $OUTPUT/kw${i}.log
			;;
	esac

	cp $PDK/boot/keywriter/x509cert/final_certificate.bin $OUTPUT/cert-kw${i}.bin
	cd $PDK/build
	echo " make keywriter_img -sj4"
	make keywriter_img -sj4 &>> $OUTPUT/kw${i}.log
	cp $PDK/boot/keywriter/binary/j721e/keywriter_img_j721e_release.tiimage $OUTPUT/kw${i}.tiimage
	cp $PDK/boot/keywriter/binary/j721e/keywriter_img_j721e_release.xer5f $OUTPUT/kw${i}.xer5f

	echo "# Done!"
done
