#include "WPILIb.h"

class Robot: public IterativeRobot
{
private:
	class VictorSP *frontLeft, *rearLeft, *frontRight, *rearRight, *intake_Motor; // Ints them as victorsp pointers
	Joystick *drivestick_left, *drivestick_right, *xbox; // Sets them to be call joystick
	RobotDrive *myDrive_all; // Sets the robotDrive class to be called by myDrive_all
	JoystickButton *Xbox_Button_A, *Xbox_Button_B, *Xbox_Button_X, *Xbox_Button_Y; //for A B X Y buttons on xbox
	std::unique_ptr<Command> autonomousCommand;// Was in default code don't know what it does
	double Xvalue_Left, Xvalue_Right, ABCheck, BBCheck, XBCheck, YBCheck, Left_Joystick_Throttle_Check, Right_Joystick_Throttle_Check; //ints all of them as double
	DoubleSolenoid *FirstSolenoid = new DoubleSolenoid(1,2); // Int DS on 1 and 2
	DigitalInput *limitSwitch = new DigitalInput(2); // Ints the limit switch to DIO 1
	Timer autoTimer;
public:
	void RobotInit()
	{
		frontLeft = new VictorSP(0), rearLeft = new VictorSP(1), frontRight = new VictorSP(2), rearRight = new VictorSP(3); // Assigns the motors to the VictorSP class
		intake_Motor = new VictorSP(5); // Assigns the motor to the VictorSP class
		myDrive_all = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight); // Has the motors be used by the robot drive class and called by myDrive_all
		drivestick_left = new Joystick(0); // 0 in joystick is the port that the joystick is plugged into on the driverstation
		drivestick_right = new Joystick(5); // 5 in joystick is the port that the joystick is plugged into on the driverstation
		xbox = new Joystick(2);// 2 in joystick is the port that the xbox is plugged into on the driverstation.
		Xbox_Button_A = new JoystickButton(xbox, 1), Xbox_Button_B = new JoystickButton(xbox, 2), Xbox_Button_X = new JoystickButton(xbox, 3), Xbox_Button_Y = new JoystickButton(xbox, 4); // Ints A B X and Y buttons on xbox
		frontRight->SetInverted(true); // All drive train motors have to be inverted
		rearRight->SetInverted(true);
		rearLeft->SetInverted(true);
		frontLeft->SetInverted(true);
	}

	void DisabledInit()
	{

	}
	void DisabledPeriodic()
	{
		Scheduler::GetInstance()->Run();
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{
		Scheduler::GetInstance()->Run(); // Was in default code don't know what it does

		if (autoTimer.Get() > 0.0 ) { // If timer is greater then zero
			if (autoTimer.Get() < 1.5 ) { // If the timer is less then 2.5
				myDrive_all->TankDrive(0.8,0.8); // Make all motors go backwards
			} else {// If the timer is greater then 2.5
				myDrive_all->TankDrive(0.0,0.0); // Make all motors stop
			}
		} else { // If the timer is less then zero
			autoTimer.Start(); // Starts timer
		}
	}

	void TeleopInit()
	{
		frontLeft->Set(0);
		frontRight->Set(0);
		rearLeft->Set(0);
		rearRight->Set(0);
		intake_Motor->Set(0);
	}

	void TeleopPeriodic(void)
		{
			Scheduler::GetInstance()->Run();

			myDrive_all->TankDrive(drivestick_left->GetY(), drivestick_right->GetY());

			ABCheck = Xbox_Button_A->Get(); // Gets the value of the A button on the xbox
			BBCheck = Xbox_Button_B->Get(); // Gets the value of the B button on the xbox
			if (ABCheck == 1) // If A button is pressed
			{
				FirstSolenoid->Set(DoubleSolenoid::kForward); // Make piston extend
			}
			if (BBCheck == 1) // If B button is pressed
			{
				FirstSolenoid->Set(DoubleSolenoid::kReverse); // Make piston retract
			}
			if ((ABCheck == 0) && (BBCheck == 0)) // If A and B buttons are not pressed
			{
				FirstSolenoid->Set(DoubleSolenoid::kOff); // Make piston stay where it is
			}

			XBCheck = Xbox_Button_X->Get(); // Gets value of the X button on the xbox
			YBCheck = Xbox_Button_Y->Get(); // Get value of the Y button on the xbox

			if (XBCheck == 1)//If X button is pressed
			{
				if (limitSwitch->Get() == 0) // If limitswitch is pressed
				{
					intake_Motor->Set(0); // Stop test motor
				}
				if (limitSwitch->Get() == 1) // If limitswitch is not pressed
				{
					intake_Motor->Set(1); // Test motor goes forward at 0.5 speed
				}
			}

			if (YBCheck == 1) // If Y button is pressed
			{
				intake_Motor->Set(-1); // Test motor goes backward at -0.5 speed
			}

			if ((XBCheck == 0) && (YBCheck == 0)) // If X and Y buttons are not pressed
			{
				intake_Motor->Set(0); // Test motor stops
			}
		}
	void TestPeriodic()
	{
		LiveWindow::GetInstance()->Run();
	}
};

START_ROBOT_CLASS(Robot)
