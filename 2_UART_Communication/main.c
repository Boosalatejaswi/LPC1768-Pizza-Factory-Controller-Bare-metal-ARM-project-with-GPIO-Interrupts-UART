2_UART_Communication/main.c
#include "LPC17xx.h"

int main() {
    // 1. Power ON GPIO
    LPC_SC->PCONP |= (1 << 15);
    
    // 2. Configure P2.0 (LED) and P2.1 (Button)
    LPC_GPIO2->FIODIR |= (1 << 0);  // Output
    LPC_GPIO2->FIODIR &= ~(1 << 1); // Input
    LPC_PINCON->PINMODE4 &= ~(3 << 2); // Pull-up
    
    while(1) {
        // 3. Order handling
        if (!(LPC_GPIO2->FIOPIN & (1 << 1))) {
            LPC_GPIO2->FIOSET = (1 << 0); // Light ON
        } else {
            LPC_GPIO2->FIOCLR = (1 << 0); // Light OFF
        }
    }
}


Part 2: Interrupt Emergency
2_Interrupt_Emergency/main.c
  #include "LPC17xx.h"

volatile int emergency_flag = 0;

void EINT3_IRQHandler() {
    emergency_flag = 1;
    LPC_GPIOINT->IO2IntClr |= (1 << 3); // Clear IRQ
}

int main() {
    // 1. Configure P2.3 as emergency button
    LPC_GPIO2->FIODIR &= ~(1 << 3);
    LPC_GPIOINT->IO2IntEnF |= (1 << 3);
    NVIC_EnableIRQ(EINT3_IRQn);
    
    while(1) {
        if(emergency_flag) {
            // Handle emergency (e.g., turn off oven)
            emergency_flag = 0;
        }
    }
}
Part 3: UART Kitchen Display
3_UART_Kitchen_Display/main.c

#include "LPC17xx.h"

void UART_Send(char* message) {
    LPC_UART0->LCR = 0x83; // DLAB=1
    LPC_UART0->DLL = 9;    // 115200 baud @100MHz PCLK
    LPC_UART0->LCR = 0x03; // 8N1 mode
    
    while(*message) {
        while(!(LPC_UART0->LSR & (1 << 5))); // Wait for THRE
        LPC_UART0->THR = *message++;
    }
}

int main() {
    LPC_SC->PCONP |= (1 << 3); // Power UART0
    LPC_PINCON->PINSEL0 |= (1 << 4) | (1 << 6); // P0.2/P0.3 as UART
    
    UART_Send("PEPPERONI ORDERED\n");
    while(1);
}
