import can
import time

def main():
    try:
        # Connect with CAN0
        bus = can.interface.Bus(channel='can0', bustype='socketcan')
        print(">>> Connection successful!\n>>> Ping to the motor with ID 1...")

        msg = can.Message(arbitration_id=1, data=[0], is_extended_id=False)
        bus.send(msg)

        print(">>> Message has been sent.\n>>>> We are waiting for response (~3 sec)...")

        start_time = time.time()
        while time.time() - start_time < 3:
            reply = bus.recv(timeout=0.5)
            if reply is not None:
                print(f">>> We receive a response!\n>>> ID: {reply.arbitration_id} | Data: {list(reply.data)}")
                return

        print("--- Timeout ---")

    except Exception as e:
        print(f"--- Error: {e} ---")

if __name__ == "__main__":
    main()
