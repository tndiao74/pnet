
// s3demo
// Copyright 2025 Raw Thrills Inc.


// pre-compile header
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////

#include "../pnet/base64.h"

#include "main.h"


///////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Internal Prototypes
///////////////////////////////////////////////////////////////////////

// great!
// now mkinitramfs ...
// sudo mkinitramfs -v -d ../iso/mkinitramfs -o ../iso/build/data/initrd.gz
// sudo sh -x /usr/sbin/mkinitramfs -d ../iso/mkinitramfs -o ../iso/build/data/initrd.gz  2> mkinitramfs.log
// ok, lets try to boot the initrd, thats what we are really kind of after
// yes, absolutely, we can move from the initrd into the encrypted load after that
// so lets just dump the gz where it is for now
// then we can spin it up in the min build for initial testing
// ok
// so, unmake the gz and look at it
// unmkinitramfs -v ../iso/build/data/initrd.gz ../iso/build/initramfs1
// ok, should we do a bootbox and do the initramfs from that
// yes
// ok, so how should we proceed with the base setup
// list commands here, restore snapshot until you get it
// transfer USB
// mkdir pnet
// mkdir pnet/bin
// mkdir pnet/misc

// 1. install the hook for mkinitramfs
// sudo cp misc/pnetb /etc/initramfs-tools/hooks/pnetb
// 2. copy bin/pnetb bin to usr/sbin
// sudo cp bin/pnetb /usr/sbin
// sudo chmod +x /usr/sbin/pnetb
// 3. install deps
// 4. test pnetb runs
// 5. copy in initramfs init script
// sudo cp misc/master-init.sh /etc/initramfs-tools/scripts/local-top/master-init.sh
// sudo chmod +x /etc/initramfs-tools/scripts/local-top/master-init.sh (stop the boot)
// 6. update-initramfs with pnetb
// sudo update-initramfs -u
// 7. grub-mkrescue -o ../iso/out/pnet.iso <- grabs up boot and packs up
// good
// again
// copy initramfs-tools
// modify
// make image
// sudo mkinitramfs -v -d ../iso/mkinitramfs -o ../iso/build/data/initrd.gz
// verify files
// sudo unmkinitramfs -v ../iso/build/data/initrd.gz ../iso/build/initramfs1
// copy grub files
// modify
// make bootable image
// sudo grub-mkimage
// 


///////////////////////////////////////////////////////////////////////
// application entry point
///////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // debug memory usage
#ifdef WINDOWS
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 1959565;
#endif

    printf("pnet client: starting now ...\n");
	
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	// Create a curl easy handle
	CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, "https://anmccall.net");

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up the easy handle
        curl_easy_cleanup(curl);
    }
	
	json jsonData;
    json params;
    json actions;
    params["mNextTick"] = 30.0f;
    int actionCnt = 3;
    params["actionCnt"] = actionCnt;
    for (int i = 0; i < actionCnt; i++)
    {
        json action;
        action["name"] = "action_name";
        action["param1"] = "action_param";
        actions += action;
    }

    jsonData += params;
    jsonData += actions;

    const size_t bufMax = 2048;
    char buf[bufMax];
    buf[0] = 0;
    base64_encode(jsonData.dump().c_str(), bufMax, buf);
    printf("%s -> %s\n", jsonData.dump().c_str(), buf);
	
	curl_global_cleanup();

#ifdef WINDOWS
    printf("Press any key to exit.\n");
    _getch();
#endif
    return 0;
}
