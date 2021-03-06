#include "Variables.h"
#include "Configuration.h"

//this is the version of our host software
unsigned int master_version = 0;

//are we paused?
bool is_tool_paused = false;

int current_temperature = 0;
int target_temperature = 0;
int max_temperature = 0;
int heater_low = 64;
int heater_high = 255;

MotorControlStyle motor1_control = MC_PWM;
volatile MotorControlDirection motor1_dir = MC_FORWARD;
volatile byte motor1_pwm = 0;
volatile long motor1_target_rpm = 0;
volatile long motor1_current_rpm = 0;
bool motor1_reversal_state = false;
int motor1_reversal_count = 0;

MotorControlStyle motor2_control = MC_PWM;
MotorControlDirection motor2_dir = MC_FORWARD;
byte motor2_pwm = 0;
long motor2_target_rpm = 0;
long motor2_current_rpm = 0;

Servo servo1;
Servo servo2;

//these are for the extruder PID
volatile int speed_error = 0;              // extruder position / error variable.
volatile int pGain = SPEED_INITIAL_PGAIN;  // Proportional gain
volatile int iGain = SPEED_INITIAL_IGAIN;  // Integral gain
volatile int dGain = SPEED_INITIAL_DGAIN;  // Derivative gain
volatile int iMax = 500;                   // Integrator max
volatile int iMin = -500;                  // Integrator min
volatile int iState = 0;                   // Integrator state
volatile int dState = 0;                   // Last position input

//what state are we in?
volatile byte stepper_index = 0;
volatile long stepper_ticks = 0;
volatile byte stepper_high_pwm = 0;
volatile byte stepper_low_pwm = 0;

boolean temp_control_enabled;
unsigned long temp_prev_time; // ms

#if TEMP_PID
float temp_pGain;
float temp_iGain;
float temp_dGain;

int  temp_dState;
long temp_iState;
float temp_iState_max; // set in update_windup
float temp_iState_min; // set in update_windup
#endif
  

