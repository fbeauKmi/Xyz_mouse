
Import("env")
board_config = env.BoardConfig()
board_config.update("build.hwids", [["0x256f", "0xC635"]])
board_config.update("upload_port.0.vid", 0x256f)
board_config.update("upload_port.0.pid", 0xc635)

board_config.update("build.usb_product", "SpaceMouse Compact")
board_config.update("vendor", "3Dconnexion")