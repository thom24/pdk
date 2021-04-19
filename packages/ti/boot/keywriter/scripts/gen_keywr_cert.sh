#!/bin/bash

##################################################################
# File: gen_keywr_cert.sh
# 
# Description: Script to generate x509 certificate for key wirter, 
#              which has encrypted customer key information.
# 
# (c) Texas Instruments 2020, All rights reserved.
##################################################################

HELPTXT="
	./gen_keywr_cert.sh - creates a x509 certificate from the keys input to this script.
	-a | --aes256
		256 bit (symmetric key)
		AES-256 key to be used (supposed to be randomly generated by customer)
		If not specified, a random 256 bit key is chosen by the script
	-b | --bmpk
		4096 bit (RSA private key, pem format)
		BMPK key to be used (not necessary, in case customer doesn't want to program
		BMPK, BMEK keys)
	--bmek
		256 bit (symmetric key, binary file)
		BMEK key to be used (not necessary, in case customer doesn't want to program
		BMPK, BMEK keys)
	-c | --cert
		Path to save final certificate
		Default is '../x509cert/final_certificate.cert'
	-g | --gen
		Generate aes256 key, bmpk, bmek, smpk, smek in keys/ folder
	-h
		Print Help Text
	-s | --smpk (mandatory)
		4096 bit (RSA private key, pem format)
		SMPK key to be used
	--smek (mandatory)
		256 bit (symmetric key, binary file)
		SMEK key to be used
	-t | --tifek (mandatory)
		4096 bit (RSA publick key, pem format)
		TI provided Forward Encryption Key

	Example Usage:
	
	# To generate keys
	./gen_keywr_cert.sh -g

	# To generate certificate
	./gen_keywr_cert.sh -s keys/smpk.pem --smek keys/smek.key -b keys/bmpk.pem --bmek keys/bmek.key -t ti_fek_public.pem -a keys/aes256.key
"

## HELP ------------------------------------------------------------
if [ "$#" -lt 1 ]; then
	echo "$HELPTXT"
	exit
fi

######################################################################################
# DECLARE TEMPORARY VARIABLES
######################################################################################

declare -A smpk_info
declare -A smek_info
declare -A bmpk_info
declare -A bmek_info

declare -A tifek_info
declare -A aes256key_info

declare -A primary_cert_info
declare -A secondary_cert_info

declare -A output_info

smpk_info[flag]="no"
smek_info[flag]="no"
bmpk_info[flag]="no"
bmek_info[flag]="no"

tifek_info[flag]="no"
aes256key_info[flag]="no"

secondary_cert_info[flag]="no"

primary_cert_info[config]="configs/prim_cert_config.txt"
secondary_cert_info[config]="configs/sec_cert_config.txt"

secondary_cert_info[file]="secondary_cert.bin"
primary_cert_info[file]="primary_cert.bin"

output_info[hash_csv]="verify_hash.csv"
output_info[certificate]="../x509cert/final_certificate.bin"

## PARSE ARGUMENTS -------------------------------------------------
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -a|--aes256)
	if [[ "${aes256key_info[flag]}" == "no" ]]; then
		aes256key_info[flag]="yes"
		aes256key_info[file]="$2"
	else
		echo "ERR: AES256 key provided multiple times"
		exit
	fi
    shift # past argument
    shift # past value
    ;;
	-t|--tifek)
	if [[ "${tifek_info[flag]}" == "no" ]]; then
		tifek_info[flag]="yes"
		tifek_info[file]="$2"
	else
		echo "ERR: TI FEK provided multiple times"
		exit
	fi
    shift # past argument
    shift # past value
    ;;
	-s|--smpk)
	if [[ "${smpk_info[flag]}" == "no" ]]; then
		smpk_info[flag]="yes"
		smpk_info[file]="$2"
	else
		echo "ERR: SMPK provided multiple times"
		exit
	fi
    shift # past argument
    shift # past value
    ;;
	--smek)
	if [[ "${smek_info[flag]}" == "no" ]]; then
		smek_info[flag]="yes"
		smek_info[file]="$2"
	else
		echo "ERR: SMEK provided multiple times"
		exit
	fi
    shift # past argument
    shift # past value
    ;;
	-b|--bmpk)
	if [[ "${bmpk_info[flag]}" == "no" ]]; then
		bmpk_info[flag]="yes"
		bmpk_info[file]="$2"
	else
		echo "ERR: BMPK provided multiple times"
		exit
	fi
    shift # past argument
    shift # past value
    ;;
	--bmek)
	if [[ "${bmek_info[flag]}" == "no" ]]; then
		bmek_info[flag]="yes"
		bmek_info[file]="$2"
	else
		echo "ERR: BMEK provided multiple times"
		exit
	fi
    shift # past argument
    shift # past value
    ;;
	-c|--cert)
	output_info[certificate]="$2"
	shift
	shift
	;;
	-h|--help)
	echo "$HELPTXT"
	exit
    shift # past argument
    shift # past value
    ;;
	-g|--gen)
	echo "# Generating dummy keys in keys/ folder"
	mkdir -p keys
	openssl rand 32 > keys/aes256.key
	openssl genrsa -out keys/smpk.pem 4096
	openssl genrsa -out keys/bmpk.pem 4096
	openssl rand 32 > keys/smek.key
	openssl rand 32 > keys/bmek.key
	exit
	shift # past argument
	shift # past value
	;;
esac
done

# Check for presence of SM*K, BM*K (optional)
if [[ "${smpk_info[flag]}" == "no" || "${smek_info[flag]}" == "no" ]]; then
	echo "ERR: SMPK, SMEK are mandatory!!"
	exit
elif [[ "${bmpk_info[flag]}" == "no" && "${bmek_info[flag]}" == "no" ]]; then
	secondary_cert_info[flag]="no"
elif [[ "${bmpk_info[flag]}" == "yes" && "${bmek_info[flag]}" == "yes" ]]; then
	secondary_cert_info[flag]="yes"
else
	if [[ "${bmpk_info[flag]}" == "no" ]]; then
		echo "ERR: BMPK missing!!"
		exit
	else
		echo "ERR: BMEK missing!!"
		exit
	fi
fi

if [[ "${secondary_cert_info[flag]}" == "yes" ]]; then
	echo "Generating Dual signed certificate!!"
else
	echo "Generating Single signed certificate!!"
fi

mkdir -p tmpdir
mkdir -p configs
mkdir -p ../x509cert


# generate a random 256 bit key -- AES256 Key
if [ "${aes256key_info[flag]}" == "no" ]; then
	echo "GEN: AES256 key generated, since not provided"
	openssl rand 32 > tmpdir/aes256.key
	aes256key_info[file]="tmpdir/aes256.key"
fi

aes256key_info[val]=$(xxd -p -c 32 "${aes256key_info[file]}")

echo "" > "${output_info[hash_csv]}"

######################################################################################
# UTILITY FUNCTIONS.
######################################################################################

# raw_encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
raw_encrypt(){
	openssl rsautl -encrypt -raw -inkey "$1" -pubin -in "$2" -out "$3"
}

# encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
encrypt(){
	openssl rsautl -encrypt -inkey "$1" -pubin -in "$2" -out "$3"
}

# sign_the_hash <PRIV-KEY.PEM> <OUTPUT> <INPUT>
sign_the_hash(){
	openssl dgst -sha512 -sign "$1" -out "$2" "$3"
}

# calc_hash <INPUT> <OUTPUT>
calc_hash(){
	openssl dgst -sha512 -binary "$1" > "$2"
}

# priv_pem_to_pub_der <INPUT> <OUTPUT>
priv_pem_to_pub_der(){
	openssl rsa -in "$1" -outform der -pubout -out "$2"
}

# gen_iv <OUTFILE>
gen_iv(){
	openssl rand -out "$1" 16
}

# gen_rs <OUTFILE>
gen_rs(){
	openssl rand -out "$1" 32
}

# gen_pub_key_x509_extension <PRIV.PEM> <PUB.DER> <PUB.HASH> <SMPKH/BMPKH> <IV> <RS> <FIELD> <OUTPUT>
gen_pub_key_x509_extension(){
	# priv_pem_to_pub_der <INPUT> <OUTPUT>
	priv_pem_to_pub_der "$1" "$2"
	# calc_hash <INPUT> <OUTPUT>
	calc_hash "$2" "$3"
	echo "$4,$(xxd -p $3)" >> "${output_info[hash_csv]}"
	# gen_iv <OUTFILE>
	gen_iv "$5"
	# gen_rs <OUTFILE>
	gen_rs "$6"
	
	IV=$(xxd -p -c 32 "$5")
	cat "$3" "$6" > "$7"
	openssl aes-256-cbc -e -K "${aes256key_info[val]}" -iv "${IV}" -nopad -in "$7" \
			-out "$8"
}

# gen_sym_key_x509_extension <SMEKH/BMEKH> <SYMKEY> <SYMKEY.HASH> <IV> <RS> <FIELD> <OUTPUT>
gen_sym_key_x509_extension(){
	# gen_iv <OUTFILE>
	gen_iv "$4"
	# gen_rs <OUTFILE>
	gen_rs "$5"
	IV=$(xxd -p -c 32 "$4")
	cat "$2" "$5" > "$6"
	openssl dgst -sha512 -binary "$2" > "$3"
	echo "$1,$(xxd -p $3)" >> "${output_info[hash_csv]}"
	openssl aes-256-cbc -e -K "${aes256key_info[val]}" -iv "${IV}" -nopad -in "$6" -out "$7"
}

# populate_config_primary
populate_config_primary(){
	ENC_AES_KEY=$(xxd -p -c 1000000 tmpdir/enc_aes_key.enc)
	sed "s/PUT_ENC_AES_KEY/${ENC_AES_KEY}/" templates/config_template.txt > "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_ENC_AES/$((${#ENC_AES_KEY}/2))/" -i "${primary_cert_info[config]}"

	ENC_SMPK_SIGNED_AES_KEY=$(xxd -p -c 1000000 tmpdir/enc_smpk_signed_aes_key.enc)
	sed -i "s/PUT_ENC_SMPK_SIGNED_AES_KEY/${ENC_SMPK_SIGNED_AES_KEY}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_ENC_SMPK_SIGNED_AES/$((${#ENC_SMPK_SIGNED_AES_KEY}/2))/" -i "${primary_cert_info[config]}"

	AESENC_SMPKH=$(xxd -p -c 1000000 tmpdir/aesenc_smpkh.enc)
	AESENC_IV_SMPKH=$(xxd -p -c 1000000 tmpdir/smpkh.iv)
	AESENC_RS_SMPKH=$(xxd -p -c 1000000 tmpdir/smpkh.rs)
	sed -i "s/PUT_AESENC_SMPKH/${AESENC_SMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_SMPKH/${AESENC_IV_SMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_SMPKH/${AESENC_RS_SMPKH}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_AESENC_SMPKH/$((${#AESENC_SMPKH}/2))/" -i "${primary_cert_info[config]}"

	AESENC_SMEK=$(xxd -p -c 1000000 tmpdir/aesenc_smek.enc)
	AESENC_IV_SMEK=$(xxd -p -c 1000000 tmpdir/smek.iv)
	AESENC_RS_SMEK=$(xxd -p -c 1000000 tmpdir/smek.rs)
	sed -i "s/PUT_AESENC_SMEK/${AESENC_SMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_SMEK/${AESENC_IV_SMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_SMEK/${AESENC_RS_SMEK}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_AESENC_SMEK/$((${#AESENC_SMEK}/2))/" -i "${primary_cert_info[config]}"
}

# populate_config_secondary
populate_config_secondary(){
	ENC_BMPK_SIGNED_AES_KEY=$(xxd -p -c 1000000 tmpdir/enc_bmpk_signed_aes_key.enc)
	sed -i "s/PUT_ENC_BMPK_SIGNED_AES_KEY/${ENC_BMPK_SIGNED_AES_KEY}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_ENC_BMPK_SIGNED_AES/$((${#ENC_BMPK_SIGNED_AES_KEY}/2))/" -i "${primary_cert_info[config]}"

	AESENC_BMPKH=$(xxd -p -c 1000000 tmpdir/aesenc_bmpkh.enc)
	AESENC_IV_BMPKH=$(xxd -p -c 1000000 tmpdir/bmpkh.iv)
	AESENC_RS_BMPKH=$(xxd -p -c 1000000 tmpdir/bmpkh.rs)
	sed -i "s/PUT_AESENC_BMPKH/${AESENC_BMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_BMPKH/${AESENC_IV_BMPKH}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_BMPKH/${AESENC_RS_BMPKH}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_AESENC_BMPKH/$((${#AESENC_BMPKH}/2))/" -i "${primary_cert_info[config]}"

	AESENC_BMEK=$(xxd -p -c 1000000 tmpdir/aesenc_bmek.enc)
	AESENC_IV_BMEK=$(xxd -p -c 1000000 tmpdir/bmek.iv)
	AESENC_RS_BMEK=$(xxd -p -c 1000000 tmpdir/bmek.rs)
	sed -i "s/PUT_AESENC_BMEK/${AESENC_BMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_IV_AESENC_BMEK/${AESENC_IV_BMEK}/" "${primary_cert_info[config]}" 
	sed -i "s/PUT_RS_AESENC_BMEK/${AESENC_RS_BMEK}/" "${primary_cert_info[config]}" 
	sed "s/PUT_SIZE_AESENC_BMEK/$((${#AESENC_BMEK}/2))/" -i "${primary_cert_info[config]}"
}

######################################################################################
# CRYPTOGRAPHIC OPERATIONS TO CALC HASH/ENC.
######################################################################################

echo "# encrypt aes256 key with tifek public part"
# encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
encrypt "${tifek_info[file]}" "${aes256key_info[file]}" tmpdir/enc_aes_key.enc

echo "# encrypt SMPK-priv signed aes256 key(hash) with tifek public part"
# sign_the_hash <PRIV-KEY.PEM> <OUTPUT> <INPUT>
sign_the_hash "${smpk_info[file]}" tmpdir/smpk_sign_aes256.sign "${aes256key_info[file]}"
# Block Size is 256 Bytes => 2048 bits
dd if=tmpdir/smpk_sign_aes256.sign of=tmpdir/smpk_sign_aes256_1.sign bs=256 count=1 status=none
dd if=tmpdir/smpk_sign_aes256.sign of=tmpdir/smpk_sign_aes256_2.sign bs=256 skip=1 count=1 status=none
# encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
encrypt "${tifek_info[file]}" tmpdir/smpk_sign_aes256_1.sign tmpdir/enc_smpk_signed_aes_key_1.enc
encrypt "${tifek_info[file]}" tmpdir/smpk_sign_aes256_2.sign tmpdir/enc_smpk_signed_aes_key_2.enc
cat tmpdir/enc_smpk_signed_aes_key_1.enc tmpdir/enc_smpk_signed_aes_key_2.enc > tmpdir/enc_smpk_signed_aes_key.enc

echo "# encrypt smpk-pub hash using aes256 key"
# gen_pub_key_x509_extension <PRIV.PEM> <PUB.DER> <PUB.HASH> <SMPKH/BMPKH> <IV> <RS> <FIELD> <OUTPUT>
gen_pub_key_x509_extension "${smpk_info[file]}" tmpdir/smpkpub.der tmpdir/smpkh "SMPKH" \
		tmpdir/smpkh.iv tmpdir/smpkh.rs tmpdir/smpkhfield tmpdir/aesenc_smpkh.enc

echo "# encrypt smek (sym key) using aes256 key"
# gen_sym_key_x509_extension <SYMKEY> <SYMKEY.HASH> <IV> <RS> <FIELD> <OUTPUT>
gen_sym_key_x509_extension "SMEKH" "${smek_info[file]}" tmpdir/smekh tmpdir/smek.iv \
		tmpdir/smek.rs tmpdir/smekfield tmpdir/aesenc_smek.enc


if [[ "${secondary_cert_info[flag]}" == "yes" ]]; then
	echo "# encrypt BMPK-priv signed aes256 key(hash) with tifek public part"
	# sign_the_hash <PRIV-KEY.PEM> <OUTPUT> <INPUT>
	sign_the_hash "${bmpk_info[file]}" tmpdir/bmpk_sign_aes256.sign "${aes256key_info[file]}"
	dd if=tmpdir/bmpk_sign_aes256.sign of=tmpdir/bmpk_sign_aes256_1.sign bs=256 count=1 status=none
	dd if=tmpdir/bmpk_sign_aes256.sign of=tmpdir/bmpk_sign_aes256_2.sign bs=256 skip=1 count=1 status=none
	# encrypt <PUBLIC-KEY.PEM> <DATA> <ENCRYPTED-OUTPUT>
	encrypt "${tifek_info[file]}" tmpdir/bmpk_sign_aes256_1.sign tmpdir/enc_bmpk_signed_aes_key_1.enc
	encrypt "${tifek_info[file]}" tmpdir/bmpk_sign_aes256_2.sign tmpdir/enc_bmpk_signed_aes_key_2.enc
	cat tmpdir/enc_bmpk_signed_aes_key_1.enc tmpdir/enc_bmpk_signed_aes_key_2.enc > tmpdir/enc_bmpk_signed_aes_key.enc
	
	echo "# encrypt bmpk-pub hash using aes256 key"
	# gen_pub_key_x509_extension <PRIV.PEM> <PUB.DER> <PUB.HASH> <SMPKH/BMPKH> <IV> <RS> <FIELD> <OUTPUT>
	gen_pub_key_x509_extension "${bmpk_info[file]}" tmpdir/bmpkpub.der tmpdir/bmpkh "BMPKH" \
			tmpdir/bmpkh.iv tmpdir/bmpkh.rs tmpdir/bmpkhfield tmpdir/aesenc_bmpkh.enc
	
	echo "# encrypt bmek (sym key) using aes256 key"
	# gen_sym_key_x509_extension <SYMKEY> <SYMKEY.HASH> <IV> <RS> <FIELD> <OUTPUT>
	gen_sym_key_x509_extension "BMEKH" "${bmek_info[file]}" tmpdir/bmekh tmpdir/bmek.iv \
			tmpdir/bmek.rs tmpdir/bmekfield tmpdir/aesenc_bmek.enc
fi

######################################################################################
# POPULATE THE CONFIG FILES WITH HASH/ENC VALUES
######################################################################################
populate_config_primary

if [[ "${secondary_cert_info[flag]}" == "yes" ]]; then
	populate_config_secondary
else
	# Delete all entries corresponding to BMPK/BMEK from the config file
	sed -i "/.*[bB][mM].[kK].*/d" "${primary_cert_info[config]}"
fi

######################################################################################
# GENERATE x509 CERTIFICATE FROM CONFIG FILE(S)
######################################################################################

# create x509 certificate, signed with SMPK
openssl req -new -x509 -key "${smpk_info[file]}" \
		-nodes -outform der -out "${primary_cert_info[file]}" \
		-config "${primary_cert_info[config]}" \
		-sha512

if [[ "${secondary_cert_info[flag]}" == "yes" ]]; then
	# create x509 certificate, dual signed with BMPK
	SHAVAL=$(openssl dgst -sha512 -hex "${primary_cert_info[file]}" | sed -e "s/^.*= //g")
	SHALEN=$(wc -c < "${primary_cert_info[file]}")
	sed "s/SEC_CERT_SHA512/${SHAVAL}/" templates/config_bmpk_template.txt > "${secondary_cert_info[config]}"
	sed -i "s/SEC_CERT_LENGTH/${SHALEN}/" "${secondary_cert_info[config]}"
	openssl req -new -x509 -key "${bmpk_info[file]}" \
		-nodes -outform der -out "${secondary_cert_info[file]}" \
		-config "${secondary_cert_info[config]}" \
		-sha512

	# Concatenate the 2 certificates 
	cat "${secondary_cert_info[file]}" "${primary_cert_info[file]}" > "${output_info[certificate]}"
	du -b "${secondary_cert_info[file]}" "${primary_cert_info[file]}" "${output_info[certificate]}"
else
	cat "${primary_cert_info[file]}" > "${output_info[certificate]}"
	du -b "${primary_cert_info[file]}" "${output_info[certificate]}"
fi

rm -rf tmpdir
echo "# SHA512 Hashes of keys are stored in ${output_info[hash_csv]} for reference.."
