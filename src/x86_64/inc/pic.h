#ifndef ARCH_X86_64_INC_PIC_H
#define ARCH_X86_64_INC_PIC_H

#define PIC1		    0x20		
#define PIC2		    0xA0	
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

void pic_disable(void);
void pic_mask_interrupts(void);
void pic_init(void);

#endif /* !ARCH_X86_64_INC_PIC_H */
