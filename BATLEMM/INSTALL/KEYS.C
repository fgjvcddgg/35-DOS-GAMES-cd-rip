#define KEYBOARD                 0x09


void (_interrupt *OldInt9)(void);

int volatile KeyScan;
char keys;

void _interrupt NewInt9( void )
{
register char keybyte;

KeyScan=inp(0x60);
keybyte=inp(0x61);

outp(0x61,(keybyte|0x80));
outp(0x61,keybyte);
outp(0x20,0x20);

if(KeyScan==75)
    keys=keys | 0x80;
if(KeyScan==72)
    keys=keys | 0x40;
if(KeyScan==80)
    keys=keys | 0x20;
if(KeyScan==77)
    keys=keys | 0x10;

if(KeyScan==203)
    keys=keys & 0x60;
if(KeyScan==200)
    keys=keys & 0xB0;
if(KeyScan==208)
    keys=keys & 0xD0;
if(KeyScan==205)
    keys=keys & 0xE0;

if(KeyScan==29)
    keys=keys | 0x08;
else
    keys=keys & 0xF7;

if(KeyScan==56)
    keys=keys | 0x04;
else
    keys=keys & 0xFB;

}

void set_key_driver( void)
{
OldInt9=_dos_getvect(KEYBOARD);
_dos_setvect(KEYBOARD,NewInt9);
}

void restore_key_driver(void)
{
_dos_setvect(KEYBOARD,OldInt9);
}
