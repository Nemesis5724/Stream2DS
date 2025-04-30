// Provide dummy TLS symbols manually (fixes missing __tls_* linker errors)
char __tls_start[4] __attribute__((section(".tdata")));
char __tls_end[4] __attribute__((section(".tdata")));
char __tdata_lma[4] __attribute__((section(".tdata")));
char __tdata_lma_end[4] __attribute__((section(".tdata")));

#include <3ds.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.0.105" // 👈 <-- CHANGE THIS!

void sendInput(int sock, const char *message) {
    send(sock, message, strlen(message), 0);
}

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    socInit(NULL, 0);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(9999);          // Must match your PC script
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    printf("Connected to PC!\n");

    circlePosition cstick;
    touchPosition touch;
    u32 kDown, kHeld;

    while (aptMainLoop()) {
        hidScanInput();
        hidCircleRead(&cstick);
        hidTouchRead(&touch);
        kDown = hidKeysDown();
        kHeld = hidKeysHeld();

        if (kDown & KEY_START) break; // Exit on Start button

        // Circle pad controls → map to WASD keys
        if (cstick.dx >  20) sendInput(sock, "key:d");
        else if (cstick.dx < -20) sendInput(sock, "key:a");
        if (cstick.dy >  20) sendInput(sock, "key:s");
        else if (cstick.dy < -20) sendInput(sock, "key:w");

        // Face buttons
        if (kHeld & KEY_A) sendInput(sock, "key:space");  // A = Space
        if (kHeld & KEY_B) sendInput(sock, "key:lshift"); // B = Shift

        // Shoulder buttons → mouse clicks
        if (kHeld & KEY_L) sendInput(sock, "click:left");  // L = Left click
        if (kHeld & KEY_R) sendInput(sock, "click:right"); // R = Right click

        // D-Pad controls → arrow keys (and escape)
        if (kHeld & KEY_DUP)    sendInput(sock, "key:up");
        if (kHeld & KEY_DDOWN)  sendInput(sock, "key:down");
        if (kHeld & KEY_DLEFT)  sendInput(sock, "key:escape"); // Left = Escape (e.g. open menu)
        if (kHeld & KEY_DRIGHT) sendInput(sock, "key:escape"); // Right = Escape

        // Touch: send mouse position (scaled ×2)
        if (kHeld & KEY_TOUCH) {
            char buf[64];
            sprintf(buf, "mouse:%d,%d", touch.px * 2, touch.py * 2);
            sendInput(sock, buf);
        }

        gspWaitForVBlank();
    }

    close(sock);
    socExit();
    gfxExit();
    return 0;
}
