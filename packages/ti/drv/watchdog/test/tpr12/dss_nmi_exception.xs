/* Use Exception */
var Exception = xdc.useModule('ti.sysbios.family.c64p.Exception');

/* Register the NMI hook */
Exception.nmiHook     = "&ESM_highpriority_interrupt";
Exception.enablePrint = false;

