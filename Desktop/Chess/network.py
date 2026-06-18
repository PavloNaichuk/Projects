import socket
import threading
import json


class NetEndpoint:
    """
    Small TCP endpoint for exchanging chess moves.

    Supported message formats:
    1. New format:
       {"start": [6, 0], "end": [4, 0], "promotion": null}

    2. Old format, still supported:
       [6, 0, 4, 0]
       [6, 0, 7, 0, "Q"]
    """

    def __init__(self, conn, on_move=None):
        self.conn = conn
        self.on_move = on_move
        self.running = True
        self._thread = threading.Thread(target=self._listen, daemon=True)
        self._thread.start()

    def _listen(self):
        buffer = b""

        while self.running:
            try:
                data = self.conn.recv(1024)
                if not data:
                    break

                buffer += data

                while b"\n" in buffer:
                    line, buffer = buffer.split(b"\n", 1)
                    if not line.strip():
                        continue

                    try:
                        start, end, promotion = self._decode_move(line)
                    except (ValueError, json.JSONDecodeError):
                        continue

                    if self.on_move:
                        self.on_move(start, end, promotion)

            except OSError:
                break
            except Exception:
                break

        self.running = False
        self.close()

    def _decode_move(self, raw_line):
        move = json.loads(raw_line.decode("utf-8"))

        if isinstance(move, dict):
            start = tuple(move["start"])
            end = tuple(move["end"])
            promotion = move.get("promotion")
        elif isinstance(move, list):
            if len(move) == 4:
                start = tuple(move[:2])
                end = tuple(move[2:4])
                promotion = None
            elif len(move) == 5:
                start = tuple(move[:2])
                end = tuple(move[2:4])
                promotion = move[4]
            else:
                raise ValueError("Invalid move list format")
        else:
            raise ValueError("Invalid move message format")

        if promotion is not None:
            promotion = str(promotion).upper()
            if promotion not in {"Q", "R", "B", "N"}:
                raise ValueError("Invalid promotion piece")

        return start, end, promotion

    def send_move(self, start, end, promotion=None):
        if promotion is not None:
            promotion = str(promotion).upper()
            if promotion not in {"Q", "R", "B", "N"}:
                raise ValueError("Invalid promotion piece")

        msg = {
            "start": list(start),
            "end": list(end),
            "promotion": promotion,
        }

        try:
            self.conn.sendall(json.dumps(msg).encode("utf-8") + b"\n")
        except OSError:
            self.running = False

    def close(self):
        self.running = False
        try:
            self.conn.close()
        except OSError:
            pass


class GameServer:
    def __init__(self, port=5000, on_move=None):
        self.on_move = on_move
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("", port))
        self.sock.listen(1)
        self.endpoint = None

        self._thread = threading.Thread(target=self._accept, daemon=True)
        self._thread.start()

    def _accept(self):
        try:
            conn, _addr = self.sock.accept()
        except OSError:
            return

        self.endpoint = NetEndpoint(conn, self.on_move)

    def send_move(self, start, end, promotion=None):
        if self.endpoint:
            self.endpoint.send_move(start, end, promotion)

    def close(self):
        if self.endpoint:
            self.endpoint.close()

        try:
            self.sock.close()
        except OSError:
            pass


class GameClient:
    def __init__(self, host, port=5000, on_move=None):
        self.on_move = on_move
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((host, port))
        self.endpoint = NetEndpoint(self.sock, self.on_move)

    def send_move(self, start, end, promotion=None):
        if self.endpoint:
            self.endpoint.send_move(start, end, promotion)

    def close(self):
        if self.endpoint:
            self.endpoint.close()

        try:
            self.sock.close()
        except OSError:
            pass
