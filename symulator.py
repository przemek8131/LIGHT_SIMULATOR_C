import sys
import json
import subprocess


def main():
    input_file = "input.json"
    output_file = "output.json"

    if len(sys.argv) >= 3:
        input_file = sys.argv[1]
        output_file = sys.argv[2]

    with open(input_file, "r") as f:
        data = json.load(f)

    proc = subprocess.Popen(
        ["LIGHT_SIMULATOR.exe"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        text=True
    )

    results = {"stepStatuses": []}

    for cmd in data["commands"]:
        command = cmd.get("type", "")
        if command == "addVehicle":
            vehicle_id = cmd.get("vehicleId", "")
            start_road = cmd.get("startRoad", "")
            end_road = cmd.get("endRoad", "")
            input_line = f"{command}\n"
            proc.stdin.write(input_line)
            input_line = f"{vehicle_id} {start_road} {end_road}\n"
            proc.stdin.write(input_line)
            proc.stdin.flush()
            output_line = proc.stdout.readline()
            # print(output_line)

        else:
            input_line = f"{command}\n"
            proc.stdin.write(input_line)
            proc.stdin.flush()
            output_line = proc.stdout.readline().strip()
            result = output_line.split() if output_line else []

            results["stepStatuses"].append({
                "leftVehicles": result
            })

    proc.stdin.close()
    proc.wait()
    with open(output_file, "w") as f:
        json.dump(results, f, indent=2)


if __name__ == "__main__":
    main()
