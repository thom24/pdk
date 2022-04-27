Welcome to the TI OTP Keywriter Software add on package!!
This package installs over the General Purpose PDK package available on ti.com

To get started please follow the below steps:

1. Install the Processor SDK RTOS from ti.com from the link here:
https://www.ti.com/tool/download/PROCESSOR-SDK-RTOS-J7200

NOTE: This package has been validated with Processor SDK RTOS 08_01_00 version 
and is the minimum SDK version required to run the OTP keywriter software. 
For any later SDK version, update the version in the path accordingly.

2. Once you have this installed, take the installer of the keywriter package and install this in the folder:

<Your SDK Installation Directory>/pdk_jacinto_08_01_00

3. Copy the OTP keywriter firmware `ti-fs-keywriter.bin` file to 

<Your SDK path>/pdk_jacinto_08_01_00/packages/ti/boot/keywriter/tifs_bin/j7200/ti-fs-keywriter.bin

4. Copy the TIFEK Public key `ti_fek_public.pem` file to 

<Your SDK path>/pdk_jacinto_08_01_00/packages/ti/boot/keywriter/scripts/ti_fek_public.pem

3. For any queries, please post your queries on e2e.ti.com
