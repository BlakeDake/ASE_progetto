void BUTTON_init(void);

typedef enum Button_Pressed {
	Not_Pressed,
	Pressed
} Button_Pressed;

extern Button_Pressed int0;
extern Button_Pressed int1;

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);
