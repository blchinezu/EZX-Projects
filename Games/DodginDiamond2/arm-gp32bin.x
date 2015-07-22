/******************************************************************************
 * Linker script for arm-elf-ld v0.1 (w) Groepaz/Hitmen
 * Target CPU: Arm/T9  Machine: GamePark GP32
 *****************************************************************************/

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)                                                  /* code entry point */

MEMORY {

    /******* gp32 internal *******/

    bootrom   : ORIGIN = 0x00000000, LENGTH = 0x00040000       /* 512k (?) bootrom => fixme */
    sysram    : ORIGIN = 0x0C000000, LENGTH = 0x0077fffc       /* 8mb (?) system memory => fixme */
                                                               /* ~0.5mb reserved for system */
    stacktop  : ORIGIN = 0x0C77fffc, LENGTH = 0x00000004       /* top of user stack => fixme */
}

SECTIONS {

  .text : {  /* CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE */

      *(.text)                           /* code */
		*(.text.*)

		  . = ALIGN(4);

		_etext = ABSOLUTE(.) ;             /* => provide adr. of end of text */

  		PROVIDE (etext = .);

    } > sysram

  .glue : AT (_etext) {

  __bglue = ABSOLUTE(.);

      *(.glue_7);                        /* ? */
      *(.glue_7t);                       /* ? */

  . = ALIGN(4);
  __eglue = ABSOLUTE(.);

} > sysram

  .tors : AT (__eglue) {  /* CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA */

		. = ALIGN(4);
    	__ctors = ABSOLUTE(.) ;
   	*(.ctors)                          /* c++ constructors */
   	. = ALIGN(4);
   	__ctors_end = ABSOLUTE(.) ;

    	__dtors = ABSOLUTE(.) ;
   	*(.dtors)                          /* c++ destructors */
   	. = ALIGN(4);
   	__dtors_end = ABSOLUTE(.) ;

  } > sysram

  .rodata : AT (__dtors_end) {  /* CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA */

        . = ALIGN(4);

      *(.rodata)                         /* initialized data */
		*(.rodata.*)
		*(.rodata1)

        . = ALIGN(4);

		_erodata = ABSOLUTE(.) ;
		_romdata = ABSOLUTE(.) ;

		PROVIDE(romdata = .);

   } > sysram

.data : AT (_romdata) {  /* CONTENTS, ALLOC, LOAD, DATA */

        . = ALIGN(4);

		_bdata = ABSOLUTE(.) ;
                _bdata = . ;
  		PROVIDE (bdata = .);

      *(.data)          /* CONTENTS, ALLOC, LOAD, DATA */
		*(.data.*)
		*(.gnu.linkonce.d*)

                 *(.data1)

        . = ALIGN(4);

                /* short initialized/writeable data field */
		*(.sdata)
		*(.sdata.*)

        . = ALIGN(4);

		*(COMMON)
		*(.COMMON)
		*(common)
		*(.common)

        . = ALIGN(4);

		_edata = ABSOLUTE(.) ;
  		PROVIDE (edata = .);

  } > sysram

        . = ALIGN(4);

        _bstart = ABSOLUTE(.) ;

  .bss (NOLOAD)  : AT (_bstart) {                      /*       non-initialized data */

		*(dynbss)
		*(.bss)
		*(.bss.*)

        . = ALIGN(4);
                /* short bss data field */
		*(.sbss)
		*(.scommon)

  } > sysram

        . = ALIGN(4);

    _end = ABSOLUTE(.) ;
    end = ABSOLUTE(.) ;  			/* needed for screwed up _sbrk ?! */

    _bend = ABSOLUTE(.) ;

  PROVIDE(bend = .);

	PROVIDE (end = .);

                                             /* the heap will be here */

  .stack (NOLOAD) : {
    	*(.stack)                              /* cpu-stack */
    	_stack = ABSOLUTE(.) ;
    } > stacktop

}
