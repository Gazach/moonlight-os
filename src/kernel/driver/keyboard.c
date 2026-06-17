#include "keyboard.h"
#include "../arch/interupt/irq.h"
#include "../arch/interupt/pic.h"
#include "../terminal.h"
#include "../arch/io.h"

#define KEY_LSHIFT    0x2A
#define KEY_RSHIFT    0x36
#define KEY_CAPS      0x3A
#define KEY_ENTER     0x1C
#define KEY_BACKSPACE 0x0E
#define KEY_ESC       0x01
#define KEY_TAB       0x0F
#define KEY_SPACE     0x39
#define KEY_LCTRL     0x1D
#define KEY_LALT      0x38

static unsigned char unshifted[][2] = {
    {0x02,'1'},{0x03,'2'},{0x04,'3'},{0x05,'4'},{0x06,'5'},
    {0x07,'6'},{0x08,'7'},{0x09,'8'},{0x0A,'9'},{0x0B,'0'},

    {0x10,'q'},{0x11,'w'},{0x12,'e'},{0x13,'r'},{0x14,'t'},
    {0x15,'y'},{0x16,'u'},{0x17,'i'},{0x18,'o'},{0x19,'p'},

    {0x1E,'a'},{0x1F,'s'},{0x20,'d'},{0x21,'f'},{0x22,'g'},
    {0x23,'h'},{0x24,'j'},{0x25,'k'},{0x26,'l'},

    {0x2C,'z'},{0x2D,'x'},{0x2E,'c'},{0x2F,'v'},
    {0x30,'b'},{0x31,'n'},{0x32,'m'},

    {0x29,'`'},
    {0x0C,'-'},{0x0D,'='},
    {0x1A,'['},{0x1B,']'},
    {0x2B,'\\'},
    {0x27,';'},{0x28,'\''},
    {0x33,','},{0x34,'.'},{0x35,'/'},

    {KEY_SPACE,' '},
    {KEY_TAB,'\t'},
    {KEY_BACKSPACE,'\b'},
    {KEY_ENTER,'\n'},

    {0,0}
};

static unsigned char shifted[][2] = {
    {0x02,'!'},{0x03,'@'},{0x04,'#'},{0x05,'$'},{0x06,'%'},
    {0x07,'^'},{0x08,'&'},{0x09,'*'},{0x0A,'('},{0x0B,')'},

    {0x10,'Q'},{0x11,'W'},{0x12,'E'},{0x13,'R'},{0x14,'T'},
    {0x15,'Y'},{0x16,'U'},{0x17,'I'},{0x18,'O'},{0x19,'P'},

    {0x1E,'A'},{0x1F,'S'},{0x20,'D'},{0x21,'F'},{0x22,'G'},
    {0x23,'H'},{0x24,'J'},{0x25,'K'},{0x26,'L'},

    {0x2C,'Z'},{0x2D,'X'},{0x2E,'C'},{0x2F,'V'},
    {0x30,'B'},{0x31,'N'},{0x32,'M'},

    {0x29,'~'},
    {0x0C,'_'},{0x0D,'+'},
    {0x1A,'{'},{0x1B,'}'},
    {0x2B,'|'},
    {0x27,':'},{0x28,'"'},
    {0x33,'<'},{0x34,'>'},{0x35,'?'},

    {KEY_SPACE,' '},
    {KEY_TAB,'\t'},
    {KEY_BACKSPACE,'\b'},
    {KEY_ENTER,'\n'},

    {0,0}
};

static int shift_pressed = 0;
static int caps_lock = 0;

static char lookup(uint8_t scancode, int shifted_mode)
{
    unsigned char (*table)[2] =
        shifted_mode ? shifted : unshifted;

    for (int i = 0; table[i][0] != 0; i++)
    {
        if (table[i][0] == scancode)
            return (char)table[i][1];
    }

    return 0;
}

static void keyboard_handler(void)
{
    uint8_t scancode = inb(0x60);

    /* Key release */
    if (scancode & 0x80)
    {
        scancode &= 0x7F;

        if (scancode == KEY_LSHIFT ||
            scancode == KEY_RSHIFT)
        {
            shift_pressed = 0;
        }

        return;
    }

    switch (scancode)
    {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            shift_pressed = 1;
            return;

        case KEY_CAPS:
            caps_lock = !caps_lock;
            return;

        case KEY_ESC:
        case KEY_LCTRL:
        case KEY_LALT:
            return;
    }

    char c = lookup(scancode,
                    shift_pressed ^ caps_lock);

    if (c)
        terminal_putchar(c);
}

void keyboard_init(void)
{
    irq_install(1, keyboard_handler);
    pic_unmask(1);
}