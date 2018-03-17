 OUTPUT_ARCH( "riscv" )
 
 ENTRY( _start )
 
 SECTIONS
 {
	 . = 0x200;

	 .text :
	 {
	 	*(.text)
	 }

	 .data :
	 {
	 	*(.data)
	 }

	 _end = .;
 }