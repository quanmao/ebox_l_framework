#ifndef __EBOX_CPU_TYPE_H
#define __EBOX_CPU_TYPE_H

/*
MCU_TYPE        MCU_PINS                                                             CLOCK    FLASH   RAM

STM32F031K6	    LQFP 32 7x7x1.4,UFQFPN 32 5x5x0.55	                                        48	32	4
STM32F030C6	    LQFP 48 7x7x1.4		                                                        48	32	4
STM32F030C8	    LQFP 48 7x7x1.4		                                                        48	64	8
STM32F030CC	    LQFP 48 7x7x1.4		                                                        48	256	32
STM32F030F4	    TSSOP 20		                                                            48	16	4
STM32F030K6	    LQFP 32 7x7x1.4	                                	                        48	32	4
STM32F030R8	    LQFP 64 10x10x1.4		                                                    48	64	8
STM32F030RC	    LQFP 64 10x10x1.4		                                                    48	256	32
STM32F031C4	    LQFP 48 7x7x1.4	                                	                        48	16	4
STM32F031C6	    LQFP 48 7x7x1.4		                                                        48	32	4
STM32F031E6	    WLCSP 25L P 0.4 MM DIE 444                                                  48	32	4
STM32F031F4	    TSSOP 20		                                                            48	16	4
STM32F031F6	    TSSOP 20		                                                            48	32	4
STM32F031G4	    UFQFPN 28 4x4x0.55		                                                    48	16	4
STM32F031G6	    UFQFPN 28 4x4x0.55		                                                    48	32	4
STM32F031K4	    UFQFPN 32 5x5x0.55		                                                    48	16	4
STM32F038C6	    LQFP 48 7x7x1.4		                                                        48	32	4
STM32F038E6	    WLCSP 25L P 0.4 MM DIE 444		                                	        48	32	4
STM32F038F6	    TSSOP 20		                                                            48	32	4
STM32F038G6	    UFQFPN 28 4x4x0.55		                                                    48	32	4
STM32F038K6	    UFQFPN 32 5x5x0.55		                                                    48	32	4
STM32F042C4	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55		                                    48	16	6
STM32F042C6	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55		                                    48	32	6
STM32F042F4	    TSSOP 20		                                                            48	16	6
STM32F042F6	    TSSOP 20		                                                            48	32	6
STM32F042G4	    UFQFPN 28 4x4x0.55		                                                    48	16	6
STM32F042G6	    UFQFPN 28 4x4x0.55		                                                    48	32	6
STM32F042K4	    LQFP 32 7x7x1.4,UFQFPN 32 5x5x0.55		                                    48	16	6
STM32F042K6	    LQFP 32 7x7x1.4,UFQFPN 32 5x5x0.55		                                    48	32	6
STM32F042T6	    WLCSP 36L DIE 445 P 0.4 MM		                                            48	32	6
STM32F048C6	    UFQFPN 48 7x7x0.55		                                                    48	32	6
STM32F048G6	    UFQFPN 28 4x4x0.55		                                                    48	32	6
STM32F048T6	    WLCSP 36L DIE 445 P 0.4 MM		                                            48	32	6
STM32F051C4	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                                        48	16	8
STM32F051C6	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                                        48	32	8
STM32F051C8	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                                        48	64	8
STM32F051K4	    LQFP 32 7x7x1.4,UFQFPN 32 5x5x0.55	                                        48	16	8
STM32F051K6	    LQFP 32 7x7x1.4,UFQFPN 32 5x5x0.55	                                        48	32	8
STM32F051K8	    LQFP 32 7x7x1.4,UFQFPN 32 5x5x0.55	                                        48	64	8
STM32F051R4	    LQFP 64 10x10x1.4		                                                    48	16	8
STM32F051R6	    LQFP 64 10x10x1.4		                                                    48	32	8
STM32F051R8	    LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM	                            48	64	8
STM32F051T8	    WLCSP 36L P 0.4 MM DIE 440		                                            48	64	8
STM32F058C8	    UFQFPN 48 7x7x0.55	                                                        48	64	8
STM32F058R8	    LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM	                            48	64	8
STM32F058T8	    WLCSP 36L P 0.4 MM DIE 440		                                            48	64	8
STM32F070C6	    LQFP 48 7x7x1.4		                                                        48	32	6
STM32F070CB	    LQFP 48 7x7x1.4		                                                        48	128	16
STM32F070F6	    TSSOP 20		                                                            48	32	6
STM32F070RB	    LQFP 64 10x10x1.4		                                                    48	128	16
STM32F071C8	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                                        48	64	16
STM32F071CB	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55,WLCSP 49L DIE 448 P 0.4 MM	            48	128	16
STM32F071RB	    LQFP 64 10x10x1.4		                                	                48	128	16
STM32F071V8	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	64	16
STM32F071VB	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	128	16
STM32F072C8	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55		                                    48	64	16
STM32F072R8	    LQFP 64 10x10x1.4		                                                    48	64	16
STM32F072RB	    LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM		                        48	128	16
STM32F072V8	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	64	16
STM32F072VB	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	128	16
STM32F078CB	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55,WLCSP 49L DIE 448 P 0.4 MM	            48	128	16
STM32F078RB	    LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM		                        48	128	16
STM32F078VB	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	128	16
STM32F091CB	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55		                                    48	128	32
STM32F091CC	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55		                                    48	256	32
STM32F091RB	    LQFP 64 10x10x1.4		                                                    48	128	32
STM32F091RC	    LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM,WLCSP 64L DIE 442 P 0.4 MM	    48	256	32
STM32F091VB	    LQFP 100 14x14x1.4		                                                    48	128	32
STM32F091VC	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	256	32
STM32F098CC	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55		                                    48	256	32
STM32F098RC	    LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM,WLCSP 64L DIE 442 P 0.4 MM	    48	256	32
STM32F098VC	    LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6		                                48	256	32


MCU_TYPE        MCU_PINS                                          CLOCK    FLASH   RAM
STM32F100C4	    LQFP 48 7x7x1.4                                         24	16	4
STM32F100C6	    LQFP 48 7x7x1.4	                                        24	32	4
STM32F100C8	    LQFP 48 7x7x1.4	                                        24	64	8
STM32F100CB	    LQFP 48 7x7x1.4		                                    24	128	8
STM32F100R4	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    24	16	4
STM32F100R6 	LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    24	32	4
STM32F100R8	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    24	64	8
STM32F100RB	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    24	128	8
STM32F100RC	    LQFP 64 10x10x1.4	                                    24	256	24
STM32F100RD	    LQFP 64 10x10x1.4	                                    24	384	32
STM32F100RE	    LQFP 64 10x10x1.4	                                    24	512	32
STM32F100V8	    LQFP 100 14x14x1.4	                                    24	64	8
STM32F100VB	    LQFP 100 14x14x1.4	                                    24	128	8
STM32F100VC	    LQFP 100 14x14x1.4	                                    24	256	24
STM32F100VD	    LQFP 100 14x14x1.4	                                    24	384	32
STM32F100VE	    LQFP 100 14x14x1.4	                                    24	512	32
STM32F100ZC	    LQFP 144 20x20x1.4	                                    24	256	24
STM32F100ZD	    LQFP 144 20x20x1.4	                                    24	384	32
STM32F100ZE	    LQFP 144 20x20x1.4	                                    24	512	32
STM32F101C4	    LQFP 48 7x7x1.4	                                        36	16	4
STM32F101C6	    LQFP 48 7x7x1.4	                                        36	32	6
STM32F101C8	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                    36	64	10
STM32F101CB	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                    36	128	16
STM32F101R4	    LQFP 64 10x10x1.4	                                    36	16	4
STM32F101R6	    LQFP 64 10x10x1.4	                                    36	32	6
STM32F101R8	    LQFP 64 10x10x1.4	                                    36	64	10
STM32F101RB	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    36	128	16
STM32F101RC	    LQFP 64 10x10x1.4	                                    36	256	32
STM32F101RD	    LQFP 64 10x10x1.4	                                    36	384	48
STM32F101RE	    LQFP 64 10x10x1.4	                                    36	512	48
STM32F101RF	    LQFP 64 10x10x1.4	                                    36	768	80
STM32F101RG	    LQFP 64 10x10x1.4	                                    36	1024	80
STM32F101T4	    VFQFPN 36 6x6x1-0	                                    36	16	4
STM32F101T6	    VFQFPN 36 6x6x1-0	                                    36	32	6
STM32F101T8	    VFQFPN 36 6x6x1-0	                                    36	64	10
STM32F101TB	    VFQFPN 36 6x6x1-0	                                    36	128	16
STM32F101V8	    LQFP 100 14x14x1.4	                                    36	64	10
STM32F101VB	    LQFP 100 14x14x1.4	                                    36	128	16
STM32F101VC	    LQFP 100 14x14x1.4	                                    36	256	32
STM32F101VD	    LQFP 100 14x14x1.4	                                    36	384	48
STM32F101VE	    LQFP 100 14x14x1.4	                                    36	512	48
STM32F101VF	    LQFP 100 14x14x1.4,WLCSP 144L DIE 470 PITCH 0.4         36	768	80
STM32F101VG	    LQFP 100 14x14x1.4	                                    36	1024	80
STM32F101ZC     LQFP 144 20x20x1.4	                                    36	256	32
STM32F101ZD	    LQFP 144 20x20x1.4	                                    36	384	48
STM32F101ZE	    LQFP 144 20x20x1.4	                                    36	512	48
STM32F101ZF	    LQFP 144 20x20x1.4	                                    36	768	80
STM32F101ZG	    LQFP 144 20x20x1.4	                                    36	1024	80
STM32F102C4	    LQFP 48 7x7x1.4	                                        48  16	4
STM32F102C6	    LQFP 48 7x7x1.4		                                    48  32	6
STM32F102C8	    LQFP 48 7x7x1.4	                                        48  64	10
STM32F102CB	    LQFP 48 7x7x1.4	                                        48  128	16
STM32F102R4	    LQFP 64 10x10x1.4	                                    48	16	4
STM32F102R6	    LQFP 64 10x10x1.4	                                    48	32	6
STM32F102R8	    LQFP 64 10x10x1.4	                                    48	64	10
STM32F102RB	    LQFP 64 10x10x1.4	                                    48	128	16
STM32F103C4	    LQFP 48 7x7x1.4	                                        72  16	6
STM32F103C6	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                    72	32	10
STM32F103C8	    LQFP 48 7x7x1.4	                                        72	64	20
STM32F103CB	    LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55	                    72	128	20
STM32F103R4	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    72	16	6
STM32F103R6	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    72	32	10
STM32F103R8	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    72	64	20
STM32F103RB	    LQFP 64 10x10x1.4,TFBGA 64 5x5x1.2	                    72	128	20
STM32F103RC	    LQFP 64 10x10x1.4,WLCSP 64	                            72	256	64
STM32F103RD	    LQFP 64 10x10x1.4,WLCSP 64	                            72	384	64
STM32F103RE	    LQFP 64 10x10x1.4,WLCSP 64	                            72	512	64
STM32F103RF	    LQFP 64 10x10x1.4	                                    72	768	96
STM32F103RG	    LQFP 64 10x10x1.4	                                    72	1024	96
STM32F103T4	    VFQFPN 36 6x6x1-0	                                    72	16	6
STM32F103T6	    VFQFPN 36 6x6x1-0	                                    72	32	10
STM32F103T8	    VFQFPN 36 6x6x1-0	                                    72	64	20
STM32F103TB	    VFQFPN 36 6x6x1-0	                                    72	128	20
STM32F103V8	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4                  72	64	20
STM32F103VB	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6	72	128	20
STM32F103VC	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4                  72	256	48
STM32F103VD	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4                  72	384	64
STM32F103VE	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4                  72	512	64
STM32F103VF	    LQFP 100 14x14x1.4	                                    72	768	96
STM32F103VG	    LQFP 100 14x14x1.4	                                    72	1024	96
STM32F103ZC	    LFBGA 144 10x10x1.7,LQFP 144 20x20x1.4                  72	256	48
STM32F103ZD	    LFBGA 144 10x10x1.7,LQFP 144 20x20x1.4                  72	384	64
STM32F103ZE	    LFBGA 144 10x10x1.7,LQFP 144 20x20x1.4                  72	512	64
STM32F103ZF	    LFBGA 144 10x10x1.7,LQFP 144 20x20x1.4                  72	768	96
STM32F103ZG	    LFBGA 144 10x10x1.7,LQFP 144 20x20x1.4                  72	1024	96
STM32F105R8	    LQFP 64 10x10x1.4	                                    72	64	64
STM32F105RB	    LQFP 64 10x10x1.4	                                    72	128	64
STM32F105RC	    LQFP 64 10x10x1.4	                                    72	256	64
STM32F105V8	    LQFP 100 14x14x1.4	                                    72	64	64
STM32F105VB	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4                  72	128	64
STM32F105VC	    LQFP 100 14x14x1.4	                                    72	256	64
STM32F107RB	    LQFP 64 10x10x1.4	                                    72	128	64
STM32F107RC	    LQFP 64 10x10x1.4	                                    72	256	64
STM32F107VB	    LQFP 100 14x14x1.4	                                    72	128	64
STM32F107VC	    LFBGA 100 10x10x1.7,LQFP 100 14x14x1.4                  72	256	64



MCU_TYPE        MCU_PINS                              CLOCK    FLASH   RAM
STM32F411VE----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----100----512----128
STM32F413VG----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----100----1024----320
STM32F429AG----UFBGA 7X7X0.6 169L P 0.5 MM----180----1024----256
STM32F401CB----UFQFPN 48 7x7x0.55----84----128----64
STM32F401CC----THIN WLCSP 49L DIE 423 PITCH 0.4,UFQFPN 48 7x7x0.55,WLCSP 49L DIE 423 R 7X7 P 0.4 MM----84----256----64
STM32F401CD----UFQFPN 48 7x7x0.55,WLCSP 49L DIE 433 P 0.4 MM----84----384----96
STM32F401CE----UFQFPN 48 7x7x0.55,WLCSP 49L DIE 433 P 0.4 MM----84----512----96
STM32F401RB----LQFP 64 10x10x1.4----84----128----64
STM32F401RC----LQFP 64 10x10x1.4----84----256----64
STM32F401RD----LQFP 64 10x10x1.4----84----384----96
STM32F401RE----LQFP 64 10x10x1.4----84----512----96
STM32F401VB----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----84----128----64
STM32F401VC----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----84----256----64
STM32F401VD----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----84----384----96
STM32F401VE----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----84----512----96
STM32F405OE----WLCSP 90 BALLS DIE 413 P 0.4 MM----168----512----(128+64)
STM32F405OG----WLCSP 90 BALLS DIE 413 P 0.4 MM----168----1024----(128+64)
STM32F405RG----LQFP 64 10x10x1.4----168----1024----(128+64)
STM32F405VG----LQFP 100 14x14x1.4----168----1024----(128+64)
STM32F405ZG----LQFP 144 20x20x1.4----168----1024----(128+64)
STM32F407IE----BGA 176,LQFP 176 24x24x1.4----168----512----(128+64)
STM32F407IG----BGA 176,LQFP 176 24x24x1.4----168----1024----(128+64)
STM32F407VE----LQFP 100 14x14x1.4----168----512----(128+64)
STM32F407VG----LQFP 100 14x14x1.4----168----1024----(128+64)
STM32F407ZE----LQFP 144 20x20x1.4----168----512----(128+64)
STM32F407ZG----LQFP 144 20x20x1.4----168----1024----(128+64)
STM32F410C8----UFQFPN 48 7x7x0.55----100----64----32
STM32F410CB----LQFP 48 7x7x1.4,UFQFPN 48 7x7x0.55----100----128----32
STM32F410R8----LQFP 64 10x10x1.4----100----64----32
STM32F410RB----LQFP 64 10x10x1.4,UFBGA 5X5X0.6 64L P 0.5 MM----100----128----32
STM32F410T8----WLCSP 36L die 458 P 0.4 MM----100----64----32
STM32F410TB----WLCSP 36L die 458 P 0.4 MM----100----128----32
STM32F411CC----UFQFPN 48 7x7x0.55,WLCSP 49L DIE 431 P 0.4 MM----100----256----128
STM32F411CE----UFQFPN 48 7x7x0.55,WLCSP 49L DIE 431 P 0.4 MM----100----512----128
STM32F411RC----LQFP 64 10x10x1.4----100----256----128
STM32F411RE----LQFP 64 10x10x1.4----100----512----128
STM32F411VC----LQFP 100 14x14x1.4----100----256----128
STM32F412CE----UFQFPN 48 7x7x0.55----100----512----256
STM32F412CG----UFQFPN 48 7x7x0.55----100----1024----256
STM32F412RE----LQFP 64 10x10x1.4,WLCSP 64L DIE 441 P 0.4 MM----100----512----256
STM32F412RG----LQFP 64 10x10x1.4,WLCSP 64L DIE 441 P 0.4 MM----100----1024----256
STM32F412VE----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----100----512----256
STM32F412VG----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----100----1024----256
STM32F412ZE----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM----100----512----256
STM32F412ZG----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM----100----1024----256
STM32F413CG----UFQFPN 48 7x7x0.55----100----1024----320
STM32F413CH----UFQFPN 48 7x7x0.55----100----1536----320
STM32F413MG----WLCSP 81L DIE 463 PITCH 0.4----100----1024----320
STM32F413MH----WLCSP 81L DIE 463 PITCH 0.4----100----1536----320
STM32F413RG----LQFP 64 10x10x1.4----100----1024----320
STM32F413RH----LQFP 64 10x10x1.4----100----1536----320
STM32F413VH----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----100----1536----320
STM32F413ZG----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM----100----1024----320
STM32F413ZH----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM----100----1536----320
STM32F415OG----WLCSP 90 BALLS DIE 413 P 0.4 MM----168----1024----(128+64)
STM32F415RG----LQFP 64 10x10x1.4----168----1024----(128+64)
STM32F415VG----LQFP 100 14x14x1.4----168----1024----(128+64)
STM32F415ZG----LQFP 144 20x20x1.4----168----1024----(128+64)
STM32F417IE----BGA 176,LQFP 176 24x24x1.4----168----512----(128+64)
STM32F417IG----BGA 176,LQFP 176 24x24x1.4----168----1024----(128+64)
STM32F417VE----LQFP 100 14x14x1.4----168----512----(128+64)
STM32F417VG----LQFP 100 14x14x1.4----168----1024----(128+64)
STM32F417ZE----LQFP 144 20x20x1.4----168----512----(128+64)
STM32F417ZG----LQFP 144 20x20x1.4----168----1024----(128+64)
STM32F423CH----UFQFPN 48 7x7x0.55----100----1536----320
STM32F423MH----WLCSP 81L DIE 463 PITCH 0.4----100----1536----320
STM32F423RH----LQFP 64 10x10x1.4----100----1536----320
STM32F423VH----LQFP 100 14x14x1.4,UFBGA 100 7x7x0.6----100----1536----320
STM32F423ZH----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM----100----1536----320
STM32F427AG----UFBGA 7X7X0.6 169L P 0.5 MM----180----1024----256
STM32F427AI----UFBGA 7X7X0.6 169L P 0.5 MM----180----2048----256
STM32F427IG----BGA 176,LQFP 176 24x24x1.4----180----1024----256
STM32F427II----BGA 176,LQFP 176 24x24x1.4----180----2048----256
STM32F427VG----LQFP 100 14x14x1.4----180----1024----256
STM32F427VI----LQFP 100 14x14x1.4----180----2048----256
STM32F427ZG----LQFP 144 20x20x1.4----180----1024----256
STM32F427ZI----LQFP 144 20x20x1.4----180----2048----256
STM32F429AI----UFBGA 7X7X0.6 169L P 0.5 MM----180----2048----256
STM32F429BE----LQFP 208 28x28x1.4----180----512----256
STM32F429BG----LQFP 208 28x28x1.4----180----1024----256
STM32F429BI----LQFP 208 28x28x1.4----180----2048----256
STM32F429IE----BGA 176,LQFP 176 24x24x1.4----180----512----256
STM32F429IG----BGA 176,LQFP 176 24x24x1.4----180----1024----256
STM32F429II----BGA 176,LQFP 176 24x24x1.4----180----2048----256
STM32F429NE----TFBGA 13X13X1.2 216L P 0.8 MM----180----512----256
STM32F429NG----TFBGA 13X13X1.2 216L P 0.8 MM----180----1024----256
STM32F429NI----TFBGA 13X13X1.2 216L P 0.8 MM----180----2048----256
STM32F429VE----LQFP 100 14x14x1.4----180----512----256
STM32F429VG----LQFP 100 14x14x1.4----180----1024----256
STM32F429VI----LQFP 100 14x14x1.4----180----2048----256
STM32F429ZE----LQFP 144 20x20x1.4----180----512----256
STM32F429ZG----LQFP 144 20x20x1.4,WLCSP 143 BALLS DIE 419 P 0.4MM----180----1024----256
STM32F429ZI----LQFP 144 20x20x1.4,WLCSP 143 BALLS DIE 419 P 0.4MM----180----2048----256
STM32F437AI----UFBGA 7X7X0.6 169L P 0.5 MM----180----2048----256
STM32F437IG----BGA 176,LQFP 176 24x24x1.4----180----1024----256
STM32F437II----BGA 176,LQFP 176 24x24x1.4----180----2048----256
STM32F437VG----LQFP 100 14x14x1.4----180----1024----256
STM32F437VI----LQFP 100 14x14x1.4----180----2048----256
STM32F437ZG----LQFP 144 20x20x1.4----180----1024----256
STM32F437ZI----LQFP 144 20x20x1.4----180----2048----256
STM32F439AI----UFBGA 7X7X0.6 169L P 0.5 MM----180----2048----256
STM32F439BG----LQFP 208 28x28x1.4----180----1024----256
STM32F439BI----LQFP 208 28x28x1.4----180----2048----256
STM32F439IG----BGA 176,LQFP 176 24x24x1.4----180----1024----256
STM32F439II----BGA 176,LQFP 176 24x24x1.4----180----2048----256
STM32F439NG----TFBGA 13X13X1.2 216L P 0.8 MM----180----1024----256
STM32F439NI----TFBGA 13X13X1.2 216L P 0.8 MM----180----2048----256
STM32F439VG----LQFP 100 14x14x1.4----180----1024----256
STM32F439VI----LQFP 100 14x14x1.4----180----2048----256
STM32F439ZG----LQFP 144 20x20x1.4,WLCSP 143 BALLS DIE 419 P 0.4MM----180----1024----256
STM32F439ZI----LQFP 144 20x20x1.4,WLCSP 143 BALLS DIE 419 P 0.4MM----180----2048----256
STM32F446MC----WLCSP 81L DIE 421 R 9X9 P 0.4 MM----180----256----128
STM32F446ME----WLCSP 81L DIE 421 R 9X9 P 0.4 MM----180----512----128
STM32F446RC----LQFP 64 10x10x1.4----180----256----128
STM32F446RE----LQFP 64 10x10x1.4----180----512----128
STM32F446VC----LQFP 100 14x14x1.4----180----256----128
STM32F446VE----LQFP 100 14x14x1.4----180----512----128
STM32F446ZC----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM,UFBGA 144 7x7x0.6----180----256----128
STM32F446ZE----LQFP 144 20x20x1.4,UFBGA 10X10 144L P 0.8 MM,UFBGA 144 7x7x0.6----180----512----128
STM32F469AE----UFBGA 7X7X0.6 169L P 0.5 MM----180----512----384
STM32F469AG----UFBGA 7X7X0.6 169L P 0.5 MM,WLCSP 168L DIE 434 12X14 P 0.4MM----180----1024----384
STM32F469AI----UFBGA 7X7X0.6 169L P 0.5 MM,WLCSP 168L DIE 434 12X14 P 0.4MM----180----2048----384
STM32F469BE----LQFP 208 28x28x1.4----180----512----384
STM32F469BG----LQFP 208 28x28x1.4----180----1024----384
STM32F469BI----LQFP 208 28x28x1.4----180----2048----384
STM32F469IE----BGA 176----180----512----384
STM32F469IG----BGA 176,LQFP 176 24x24x1.4----180----1024----384
STM32F469II----BGA 176,LQFP 176 24x24x1.4----180----2048----384
STM32F469NE----TFBGA 13X13X1.2 216L P 0.8 MM----180----512----384
STM32F469NG----TFBGA 13X13X1.2 216L P 0.8 MM----180----1024----384
STM32F469NI----TFBGA 13X13X1.2 216L P 0.8 MM----180----2048----384
STM32F469VE----LQFP 100 14x14x1.4----180----512----384
STM32F469VG----LQFP 100 14x14x1.4----180----1024----384
STM32F469VI----LQFP 100 14x14x1.4----180----2048----384
STM32F469ZE----LQFP 144 20x20x1.4----180----512----384
STM32F469ZG----LQFP 144 20x20x1.4----180----1024----384
STM32F469ZI----LQFP 144 20x20x1.4----180----2048----384
STM32F479AG----UFBGA 7X7X0.6 169L P 0.5 MM----180----1024----384
STM32F479AI----UFBGA 7X7X0.6 169L P 0.5 MM,WLCSP 168L DIE 434 12X14 P 0.4MM----180----2048----384
STM32F479BG----LQFP 208 28x28x1.4----180----1024----384
STM32F479BI----LQFP 208 28x28x1.4----180----2048----384
STM32F479IG----BGA 176----180----1024----384
STM32F479II----BGA 176,LQFP 176 24x24x1.4----180----2048----384
STM32F479NG----TFBGA 13X13X1.2 216L P 0.8 MM----180----1024----384
STM32F479NI----TFBGA 13X13X1.2 216L P 0.8 MM----180----2048----384
STM32F479VG----LQFP 100 14x14x1.4----180----1024----384
STM32F479VI----LQFP 100 14x14x1.4----180----2048----384
STM32F479ZG----LQFP 144 20x20x1.4----180----1024----384
STM32F479ZI----LQFP 144 20x20x1.4----180----2048----384
*/

typedef enum
{
    STM32F031K6 = 0,
    STM32F030C6,
    STM32F030C8,
    STM32F030CC,
    STM32F030F4,
    STM32F030K6,
    STM32F030R8,
    STM32F030RC,
    STM32F031C4,
    STM32F031C6,
    STM32F031E6,
    STM32F031F4,
    STM32F031F6,
    STM32F031G4,
    STM32F031G6,
    STM32F031K4,
    STM32F038C6,
    STM32F038E6,
    STM32F038F6,
    STM32F038G6,
    STM32F038K6,
    STM32F042C4,
    STM32F042C6,
    STM32F042F4,
    STM32F042F6,
    STM32F042G4,
    STM32F042G6,
    STM32F042K4,
    STM32F042K6,
    STM32F042T6,
    STM32F048C6,
    STM32F048G6,
    STM32F048T6,
    STM32F051C4,
    STM32F051C6,
    STM32F051C8,
    STM32F051K4,
    STM32F051K6,
    STM32F051K8,
    STM32F051R4,
    STM32F051R6,
    STM32F051R8,
    STM32F051T8,
    STM32F058C8,
    STM32F058R8,
    STM32F058T8,
    STM32F070C6,
    STM32F070CB,
    STM32F070F6,
    STM32F070RB,
    STM32F071C8,
    STM32F071CB,
    STM32F071RB,
    STM32F071V8,
    STM32F071VB,
    STM32F072C8,
    STM32F072R8,
    STM32F072RB,
    STM32F072V8,
    STM32F072VB,
    STM32F078CB,
    STM32F078RB,
    STM32F078VB,
    STM32F091CB,
    STM32F091CC,
    STM32F091RB,
    STM32F091RC,
    STM32F091VB,
    STM32F091VC,
    STM32F098CC,
    STM32F098RC,
    STM32F098VC,

    STM32F100C4 = 100,
    STM32F100C6,
    STM32F100C8,
    STM32F100CB,
    STM32F100R4,
    STM32F100R6,
    STM32F100R8,
    STM32F100RB,
    STM32F100RC,
    STM32F100RD,
    STM32F100RE,
    STM32F100V8,
    STM32F100VB,
    STM32F100VC,
    STM32F100VD,
    STM32F100VE,
    STM32F100ZC,
    STM32F100ZD,
    STM32F100ZE,
    STM32F101C4,
    STM32F101C6,
    STM32F101C8,
    STM32F101CB,
    STM32F101R4,
    STM32F101R6,
    STM32F101R8,
    STM32F101RB,
    STM32F101RC,
    STM32F101RD,
    STM32F101RE,
    STM32F101RF,
    STM32F101RG,
    STM32F101T4,
    STM32F101T6,
    STM32F101T8,
    STM32F101TB,
    STM32F101V8,
    STM32F101VB,
    STM32F101VC,
    STM32F101VD,
    STM32F101VE,
    STM32F101VF,
    STM32F101VG,
    STM32F101ZC,
    STM32F101ZD,
    STM32F101ZE,
    STM32F101ZF,
    STM32F101ZG,
    STM32F102C4,
    STM32F102C6,
    STM32F102C8,
    STM32F102CB,
    STM32F102R4,
    STM32F102R6,
    STM32F102R8,
    STM32F102RB,
    STM32F103C4,
    STM32F103C6,
    STM32F103C8,
    STM32F103CB,
    STM32F103R4,
    STM32F103R6,
    STM32F103R8,
    STM32F103RB,
    STM32F103RC,
    STM32F103RD,
    STM32F103RE,
    STM32F103RF,
    STM32F103RG,
    STM32F103T4,
    STM32F103T6,
    STM32F103T8,
    STM32F103TB,
    STM32F103V8,
    STM32F103VB,
    STM32F103VC,
    STM32F103VD,
    STM32F103VE,
    STM32F103VF,
    STM32F103VG,
    STM32F103ZC,
    STM32F103ZD,
    STM32F103ZE,
    STM32F103ZF,
    STM32F103ZG,
    STM32F105R8,
    STM32F105RB,
    STM32F105RC,
    STM32F105V8,
    STM32F105VB,
    STM32F105VC,
    STM32F107RB,
    STM32F107RC,
    STM32F107VB,
    STM32F107VC,

    STM32F373VC = 300,

    STM32F411VE = 400,
    STM32F413VG,
    STM32F429AG,
    STM32F401CB,
    STM32F401CC,
    STM32F401CD,
    STM32F401CE,
    STM32F401RB,
    STM32F401RC,
    STM32F401RD,
    STM32F401RE,
    STM32F401VB,
    STM32F401VC,
    STM32F401VD,
    STM32F401VE,
    STM32F405OE,
    STM32F405OG,
    STM32F405RG,
    STM32F405VG,
    STM32F405ZG,
    STM32F407IE,
    STM32F407IG,
    STM32F407VE,
    STM32F407VG,
    STM32F407ZE,
    STM32F407ZG,
    STM32F410C8,
    STM32F410CB,
    STM32F410R8,
    STM32F410RB,
    STM32F410T8,
    STM32F410TB,
    STM32F411CC,
    STM32F411CE,
    STM32F411RC,
    STM32F411RE,
    STM32F411VC,
    STM32F412CE,
    STM32F412CG,
    STM32F412RE,
    STM32F412RG,
    STM32F412VE,
    STM32F412VG,
    STM32F412ZE,
    STM32F412ZG,
    STM32F413CG,
    STM32F413CH,
    STM32F413MG,
    STM32F413MH,
    STM32F413RG,
    STM32F413RH,
    STM32F413VH,
    STM32F413ZG,
    STM32F413ZH,
    STM32F415OG,
    STM32F415RG,
    STM32F415VG,
    STM32F415ZG,
    STM32F417IE,
    STM32F417IG,
    STM32F417VE,
    STM32F417VG,
    STM32F417ZE,
    STM32F417ZG,
    STM32F423CH,
    STM32F423MH,
    STM32F423RH,
    STM32F423VH,
    STM32F423ZH,
    STM32F427AG,
    STM32F427AI,
    STM32F427IG,
    STM32F427II,
    STM32F427VG,
    STM32F427VI,
    STM32F427ZG,
    STM32F427ZI,
    STM32F429AI,
    STM32F429BE,
    STM32F429BG,
    STM32F429BI,
    STM32F429IE,
    STM32F429IG,
    STM32F429II,
    STM32F429NE,
    STM32F429NG,
    STM32F429NI,
    STM32F429VE,
    STM32F429VG,
    STM32F429VI,
    STM32F429ZE,
    STM32F429ZG,
    STM32F429ZI,
    STM32F437AI,
    STM32F437IG,
    STM32F437II,
    STM32F437VG,
    STM32F437VI,
    STM32F437ZG,
    STM32F437ZI,
    STM32F439AI,
    STM32F439BG,
    STM32F439BI,
    STM32F439IG,
    STM32F439II,
    STM32F439NG,
    STM32F439NI,
    STM32F439VG,
    STM32F439VI,
    STM32F439ZG,
    STM32F439ZI,
    STM32F446MC,
    STM32F446ME,
    STM32F446RC,
    STM32F446RE,
    STM32F446VC,
    STM32F446VE,
    STM32F446ZC,
    STM32F446ZE,
    STM32F469AE,
    STM32F469AG,
    STM32F469AI,
    STM32F469BE,
    STM32F469BG,
    STM32F469BI,
    STM32F469IE,
    STM32F469IG,
    STM32F469II,
    STM32F469NE,
    STM32F469NG,
    STM32F469NI,
    STM32F469VE,
    STM32F469VG,
    STM32F469VI,
    STM32F469ZE,
    STM32F469ZG,
    STM32F469ZI,
    STM32F479AG,
    STM32F479AI,
    STM32F479BG,
    STM32F479BI,
    STM32F479IG,
    STM32F479II,
    STM32F479NG,
    STM32F479NI,
    STM32F479VG,
    STM32F479VI,
    STM32F479ZG,
    STM32F479ZI,

} CpuType;

#endif
