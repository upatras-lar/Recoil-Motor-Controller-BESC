import can
import time

def main():
    try:
        bus = can.interface.Bus(channel='can0', interface='socketcan')
        print("### Bypass Watchdog starting...")

        reset_msg = can.Message(arbitration_id=4, data=[0, 4], is_extended_id=False)
        bus.send(reset_msg)
        time.sleep(0.005)

        cal_msg = can.Message(arbitration_id=4, data=[2, 4], is_extended_id=False)
        bus.send(cal_msg)
        print("### Calibration Mode...\n>>> Check the motor!")

    except Exception as e:
        print(f"--- Error: {e} ---")

if __name__ == "__main__":
    main()
