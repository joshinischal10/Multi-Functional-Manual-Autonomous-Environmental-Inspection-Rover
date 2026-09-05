from dataclasses import dataclass


@dataclass
class SensorData:
    motion: bool
    temperature: float
    humidity: float
    gas_value: int

    @classmethod
    def from_serial_line(cls, line: str):
        parts = line.split(",")

        if len(parts) != 5 or parts[0] != "DATA":
            raise ValueError("Expected DATA,motion,temperature,humidity,gas")

        motion = parts[1] == "1"
        temperature = float(parts[2])
        humidity = float(parts[3])
        gas_value = int(parts[4])

        return cls(
            motion=motion,
            temperature=temperature,
            humidity=humidity,
            gas_value=gas_value,
        )

    def __str__(self):
        motion_status = "Detected" if self.motion else "None"

        return (
            f"Motion: {motion_status} | "
            f"Temperature: {self.temperature:.1f} C | "
            f"Humidity: {self.humidity:.1f}% | "
            f"Gas: {self.gas_value}"
        )
