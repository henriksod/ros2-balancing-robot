import rclpy
from rclpy.node import Node

import yaml
from pathlib import Path
from can import ASCWriter
from can.message import Message

from balancing_robot.balancing_robot_msgs.msg import CANMessage


class SerialCANLogger(Node):
    def __init__(self):
        super().__init__("serial_can_logger")
        self.get_logger().info("This node takes CANMessage topics and writes to .asc files")

        with open("nodes/serial_can_logger/config_data.yaml", "r") as f:
            config = yaml.safe_load(f)

        log_file = Path(config["log_file"]).resolve()
        log_file.parent.mkdir(parents=True, exist_ok=True)
        self.get_logger().info(f"Starting logging to file {log_file}")

        self._fh = open(str(log_file), "w")
        self._writer = ASCWriter(self._fh)

        self.subscription = self.create_subscription(
            CANMessage, "can_data", self.on_message_received, 10
        )
        self.subscription  # prevent unused variable warning

    def on_message_received(self, msg):
        # self.get_logger().info(f"Received {msg}")
        self._writer.on_message_received(self.convert(msg))

    def convert(self, msg: CANMessage) -> Message:
        return Message(
            timestamp=float(msg.timestamp),
            arbitration_id=msg.arbitration_id,
            dlc=msg.dlc,
            data=msg.payload,
            is_error_frame=msg.invalid,
        )

    def destroy_node(self):
        self._writer.stop()
        self._fh.close()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = SerialCANLogger()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
