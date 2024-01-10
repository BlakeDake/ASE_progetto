void BUTTON_init(void);

enum INT0 {
	Not_Pressed,
	Pressed
};
extern enum INT0 int0;

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);
