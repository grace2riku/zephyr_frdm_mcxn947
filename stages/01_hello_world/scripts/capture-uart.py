"""J17のUARTを115200 bps / 8N1 / フロー制御なしで記録する。"""

import argparse
from pathlib import Path
import time

import serial


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", required=True)
    parser.add_argument("--seconds", type=float, default=30)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if args.seconds <= 0:
        parser.error("--seconds must be positive")

    # open前にDTR/RTSを設定し、受信専用で使用する。
    port = serial.Serial(
        baudrate=115200, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE, timeout=0.2,
        xonxoff=False, rtscts=False, dsrdtr=False,
    )
    port.dtr = False
    port.rts = False
    port.port = args.port
    with port:
        print(f"READY: {args.port}, 115200, 8N1, flow control none", flush=True)
        deadline = time.monotonic() + args.seconds
        received = bytearray()
        while time.monotonic() < deadline:
            received.extend(port.read(max(1, port.in_waiting)))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(received)
    print(received.decode("utf-8", errors="replace"), end="", flush=True)
    print(f"\nSaved: {args.output} ({len(received)} bytes)", flush=True)
    expected = b"Hello World! frdm_mcxn947/mcxn947/cpu0"
    if expected not in received:
        raise SystemExit("Expected Hello World line was not received.")


if __name__ == "__main__":
    main()
