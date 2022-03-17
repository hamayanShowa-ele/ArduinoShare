/**********************************************************/
/* try the multi task system on AVR micro computer.       */
/**********************************************************/
#include  <mul_tsk.h>
#include  <MY_STREAM.h>

extern "C"
{
//  #include <mul_tsk.h>
}

enum TSK_ID_NUMBERS
{
  ID_task1,
  ID_task2,
  ID_task3,
  ID_stack_monitor,
  ID_prnTask,
  TSKID_END,
};

/**********************************************************/
/* prototypes                                             */
/**********************************************************/
void tsk_ini( void );
void stackMonitor( void );
void task123( void );
void prnTask( void );
static void seriOutput( const char *str );

/**********************************************************/
/* valiables                                              */
/**********************************************************/
MY_STREAM serialTxFifo(256);

static uint8_t tsk0_stk[ 128 * 5 ];
static uint8_t tsk1_stk[ 128 * 5 ];
static uint8_t tsk2_stk[ 128 * 5 ];
static uint8_t tsk3_stk[ 128 * 5 ];
static uint8_t tsk4_stk[ 128 * 2 ];

/**********************************************************/
/* initialize tasks                                       */
/**********************************************************/
void tsk_ini( void )
{
  cfg_sys( TSKID_END );

  reg_tsk( ID_stack_monitor, (void *)stackMonitor, (void *)tsk0_stk, sizeof(tsk0_stk), 0,0,0,0 );
  reg_tsk( ID_task1, (void *)task123, (void *)tsk1_stk, sizeof(tsk1_stk), 1,300UL,0,0 );
  reg_tsk( ID_task2, (void *)task123, (void *)tsk2_stk, sizeof(tsk2_stk), 2,400UL,0,0 );
  reg_tsk( ID_task3, (void *)task123, (void *)tsk3_stk, sizeof(tsk3_stk), 3,500UL,0,0 );
  reg_tsk( ID_prnTask, (void *)prnTask, (void *)tsk4_stk, sizeof(tsk4_stk), 0,0,0,0 );

  sta_tsk( ID_stack_monitor );
//  sta_tsk( ID_task1 );
//  sta_tsk( ID_task2 );
//  sta_tsk( ID_task3 );
//  sta_tsk( ID_prnTask );
}

/**********************************************************/
/* setup                                                  */
/**********************************************************/
void setup()
{
  /* Operation of the port for the USB to be recognized by the PC. */
  pinMode( PC13, OUTPUT );  // 
  digitalWrite( PC13, LOW );  // 
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, LOW );

  Serial.begin( 115200 );
  while( !Serial ) { delay( 20 ); }
  String msg = "STM32 Arduino Multi Task Demo. " + String( millis() ) + "ms";
  Serial.println( msg );
  delay( 1000UL );

  tsk_ini();  // Initialize Tasks
  sta_rdq( ID_stack_monitor );  // Round robin starts. The task starts here.

  /* Infinite loop */
  while(1) {}
}

/**********************************************************/
/* loop,,, but not reached.                               */
/**********************************************************/
void loop() {}

/**********************************************************/
/* stack monitor                                          */
/**********************************************************/
static uint32_t RemainStack( void *stk, uint32_t sz )
{
  uint32_t i;
  char *ptr = (char *)stk;
  for( i = 0; i < sz; i++ ) { if( *ptr++ != 0 ) break; }
  return sz - i;
}

void stackMonitor( void )
{
  sta_tsk( ID_prnTask );
  sta_tsk( ID_task1 );
  sta_tsk( ID_task2 );
  sta_tsk( ID_task3 );

  while( 1 )
  {
    /*stack report.*/
    dly_tsk( 10 * 1000UL );
    char msg[64];
    sprintf( msg, "  TASK1:%d/%d", RemainStack( tsk1_stk, sizeof(tsk1_stk) ), sizeof(tsk1_stk) );
    seriOutput( (const char *)msg );

    sprintf( msg, "  TASK2:%d/%d", RemainStack( tsk2_stk, sizeof(tsk2_stk) ), sizeof(tsk2_stk) );
    seriOutput( (const char *)msg );

    sprintf( msg, "  TASK3:%d/%d", RemainStack( tsk3_stk, sizeof(tsk3_stk) ), sizeof(tsk3_stk) );
    seriOutput( (const char *)msg );

    sprintf( msg, "  PRN TASK:%d/%d", RemainStack( tsk4_stk, sizeof(tsk4_stk) ), sizeof(tsk4_stk) );
    seriOutput( (const char *)msg );

    sprintf( msg, "  MONITOR:%d/%d", RemainStack( tsk0_stk, sizeof(tsk0_stk) ), sizeof(tsk0_stk) );
    seriOutput( (const char *)msg );
  }
}

/**********************************************************/
/* task1,2,3                                              */
/**********************************************************/
void task123( void )
{
  // Get startup arguments.
  VP_INT exinf[2];
  get_par( exinf, sizeof(exinf) / sizeof(exinf[0]) );

  while( 1 )
  {
    // Do not use the String type, but do well with the char type.
    // String type may not be reentrant.
    char msg[64];
    sprintf( msg, "this is task%d. %ums", exinf[0], millis() );
    seriOutput( (const char *)msg );
    dly_tsk( exinf[1] );
    digitalWrite( LED_BUILTIN, ( digitalRead(LED_BUILTIN) == LOW ) ? HIGH : LOW );
  }
}

/**********************************************************/
/* serial output task                                     */
/**********************************************************/
void prnTask( void )
{
  while( 1 )
  {
    int c = serialTxFifo.read();
    if( c >= 0 )
    {
      while( Serial.availableForWrite() == 0 ) { rot_rdq(); }
      Serial.write( c );
    }
    rot_rdq();
  }
}

/**********************************************************/
/* serial output                                          */
/**********************************************************/
static void seriOutput( const char *str )
{
  while( *str ) { serialTxFifo.write( *str++ ); }
  serialTxFifo.write( '\r' );
  serialTxFifo.write( '\n' );
}
