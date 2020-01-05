#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#include <switch.h>

#define IRAM_PAYLOAD_MAX_SIZE 0x2F000
#define IRAM_PAYLOAD_BASE 0x40010000

static alignas(0x1000) u8 g_reboot_payload[IRAM_PAYLOAD_MAX_SIZE];
static alignas(0x1000) u8 g_ff_page[0x1000];
static alignas(0x1000) u8 g_work_page[0x1000];

void do_iram_dram_copy(void *buf, uintptr_t iram_addr, size_t size, int option) {
    memcpy(g_work_page, buf, size);
    
    SecmonArgs args = {0};
    args.X[0] = 0xF0000201;              /* smcAmsIramCopy */
    args.X[1] = (uintptr_t)g_work_page;  /* DRAM Address */
    args.X[2] = iram_addr;               /* IRAM Address */
    args.X[3] = size;                    /* Copy size */
    args.X[4] = option;                  /* 0 = Read, 1 = Write */
    svcCallSecureMonitor(&args);
    
    memcpy(buf, g_work_page, size);
}

void copy_to_iram(uintptr_t iram_addr, void *buf, size_t size) {
    do_iram_dram_copy(buf, iram_addr, size, 1);
}

void copy_from_iram(void *buf, uintptr_t iram_addr, size_t size) {
    do_iram_dram_copy(buf, iram_addr, size, 0);
}

static void clear_iram(void) {
    memset(g_ff_page, 0xFF, sizeof(g_ff_page));
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += sizeof(g_ff_page)) {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, g_ff_page, sizeof(g_ff_page));
    }
}

static void reboot_to_payload(void) {
    clear_iram();
    
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += 0x1000) {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, &g_reboot_payload[i], 0x1000);
    }
    
    splSetConfig((SplConfigItem)65001, 2);
}

void swap_qmenu_ulaunch(void) {    

//    unsigned char file_qlaunch[] = "/atmosphere/contents/0100000000001000/exefs.nsp";
//    unsigned char file_ShopN[] = "/atmosphere/contents/010000000000100B/exefs.nsp";
//    unsigned char file_LibraryAppletAuth[] = "/atmosphere/contents/0100000000001001/exefs.nsp";
//    unsigned char file_flog[] = "/atmosphere/contents/01008BB00013C000/exefs.nsp";
    unsigned char file_1000[] = "/ulaunch/ProtonELV/0100000000001000/exefs.nsp";
//   unsigned char file_100B[] = "/ulaunch/ProtonELV/010000000000100B/exefs.nsp";
//   unsigned char file_1001[] = "/ulaunch/ProtonELV/0100000000001001/exefs.nsp";
    unsigned char file_C000_bak[] = "/ulaunch/ProtonELV/backup/01008BB00013C000/exefs.nsp";
    unsigned char file_C000_cont[] = "/atmosphere/contents/01008BB00013C000/exefs.nsp";
   
    int lState = 0;  
  
    if (!access((const char *)file_1000, F_OK)) {
        lState = 1; // first start 
    } else if ((!access((const char *)file_C000_cont, F_OK)) || (!access((const char *)file_C000_bak, F_OK))) {
        lState = 3; // can swap
    } else {
        lState = 2; // error no uLaunch  
    }
    
    switch (lState) {
        case 1:
            printf("Ready for liftoff!\nHere's to successful (u)launch! Poyekhali!\nReboot to payload now.\n");
            mkdir("/ulaunch/ProtonELV/backup", 0700);
            rename("/atmosphere/contents/0100000000001000/", "/ulaunch/ProtonELV/backup/0100000000001000/");
            rename("/atmosphere/contents/010000000000100B/", "/ulaunch/ProtonELV/backup/010000000000100B/");
            rename("/atmosphere/contents/0100000000001001/", "/ulaunch/ProtonELV/backup/0100000000001001/");
            rename("/atmosphere/contents/01008BB00013C000/", "/ulaunch/ProtonELV/backup/01008BB00013C000/");
            rename("/ulaunch/ProtonELV/0100000000001000/", "/atmosphere/contents/0100000000001000/");
            rename("/ulaunch/ProtonELV/010000000000100B/", "/atmosphere/contents/010000000000100B/");
            rename("/ulaunch/ProtonELV/0100000000001001/", "/atmosphere/contents/0100000000001001/");
            rename("/ulaunch/ProtonELV/01008BB00013C000/", "/atmosphere/contents/01008BB00013C000/");
            break;
                
        case 2:
            printf("China won the space race...\nFile [%s] access failed: %d\nThe only thing left is to pray.\n", file_1000, errno);
            break; 

        case 3: 
            printf("Swapped.\nReboot to payload for the changes to apply.\n\n");
            rename("/atmosphere/contents/0100000000001000/", "/ulaunch/ProtonELV/0100000000001000temp/");
            rename("/ulaunch/ProtonELV/backup/0100000000001000/", "/atmosphere/contents/0100000000001000/");
            rename("/ulaunch/ProtonELV/0100000000001000temp/", "/ulaunch/ProtonELV/backup/0100000000001000/");

            rename("/atmosphere/contents/010000000000100B/", "/ulaunch/ProtonELV/010000000000100Btemp/");
            rename("/ulaunch/ProtonELV/backup/010000000000100B/", "/atmosphere/contents/010000000000100B/");
            rename("/ulaunch/ProtonELV/010000000000100Btemp/", "/ulaunch/ProtonELV/backup/010000000000100B/");

            rename("/atmosphere/contents/0100000000001001/", "/ulaunch/ProtonELV/0100000000001001temp/");
            rename("/ulaunch/ProtonELV/backup/0100000000001001/", "/atmosphere/contents/0100000000001001/");
            rename("/ulaunch/ProtonELV/0100000000001001temp/", "/ulaunch/ProtonELV/backup/0100000000001001/");

            rename("/atmosphere/contents/01008BB00013C000/", "/ulaunch/ProtonELV/01008BB00013C000temp/");
            rename("/ulaunch/ProtonELV/backup/01008BB00013C000/", "/atmosphere/contents/01008BB00013C000/");
            rename("/ulaunch/ProtonELV/01008BB00013C000temp/", "/ulaunch/ProtonELV/backup/01008BB00013C000/");
            break;
    }
/*    switch (lState) {
        case 1: 
            printf("qlaunch is active, swapped with uLaunch.\n\n");
            if (!rename("/atmosphere/contents/0100000000001000/", "/atmosphere/contents/0100000000001000qlaunch/")) {
                rename("/atmosphere/contents/0100000000001000ulaunch/", "/atmosphere/contents/0100000000001000/");             
            } else {
                mkdir("/atmosphere/contents/0100000000001000qlaunch/", 0700);
                rename("/atmosphere/contents/0100000000001000ulaunch/", "/atmosphere/contents/0100000000001000/");
            }
            break;
        case 2: 
            printf("File [%s] access failed: %d\nConsult the information at the top of the screen to set things up.\n\n", file_u, errno);
            break;
        case 3:
            printf("uLaunch is active, swapped with qlaunch.\n\n");
            rename("/atmosphere/contents/0100000000001000/", "/atmosphere/contents/0100000000001000ulaunch/");
            rename("/atmosphere/contents/0100000000001000qlaunch/", "/atmosphere/contents/0100000000001000/");
            break;
    } */   
}   
 
int main(int argc, char **argv)
{
    consoleInit(NULL);
    printf("First setup/update:\nplace four folders from release: '/atmosphere/contents'\ninto '/ulaunch/ProtonELV' (create).\n\nPress [X] to swap qlaunch with uLaunch\n");
    
bool can_reboot = true;
    Result rc = splInitialize();
    if (R_FAILED(rc)) {
        printf("Failed to initialize spl: 0x%x\n", rc);
        can_reboot = false;
    } else {
        FILE *f = fopen("sdmc:/atmosphere/reboot_payload.bin", "rb");
        if (f == NULL) {
            printf("Failed to open atmosphere/reboot_payload.bin!\n");
            can_reboot = false;
        } else {
            fread(g_reboot_payload, 1, sizeof(g_reboot_payload), f);
            fclose(f);
            printf("Press [-] to reboot to payload\n");
        }
    }
        
    printf("Press [L] to exit\n\n");

    // Main loop
    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        u64 kDown = 0;

        for (int controller = 0; controller < 10; controller++) {
            // hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
            kDown |= hidKeysDown((HidControllerID) controller);
        }

        if (can_reboot && kDown & KEY_MINUS) {
            reboot_to_payload();
        }
        if (kDown & KEY_L)  { break; } // break in order to return to hbmenu 
        
        if (kDown & KEY_X) {
            swap_qmenu_ulaunch();
        }
        
        consoleUpdate(NULL);
    }

    if (can_reboot) {
        splExit();
    }
    
    consoleExit(NULL);
    return 0;
}
