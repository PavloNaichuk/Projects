import socket
import threading
import json

class NetEndpoint:
    def __init__(self, conn, on_move):
        self.conn = conn
        self.on_move = on_move
        self.running = True
        threading.Thread(target=self._listen, daemon=True).start()

    def _listen(self):
        buf = b''
        while self.running:
            try:
                data = self.conn.recv(1024)
                if not data:
                    break
                buf += data
                while b'\n' in buf:
                    line, buf = buf.split(b'\n', 1)
                    move = json.loads(line.decode())
                    self.on_move(tuple(move[:2]), tuple(move[2:]))
            except Exception:
                break
        self.running = False
        try:
            self.conn.close()
        except:
            pass

    def send_move(self, start, end):
        msg = json.dumps([*start, *end]).encode() + b'\n'
        try:
            self.conn.sendall(msg)
        except:
            pass

    def close(self):
        self.running = False
        try:
            self.conn.close()
        except:
            pass

class GameServer:
    def __init__(self, port=5000, on_move=None):
        self.on_move = on_move
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(('', port))
        self.sock.listen(1)
        self.endpoint = None
        threading.Thread(target=self._accept, daemon=True).start()

    def _accept(self):
        try:
            conn, addr = self.sock.accept()
        except OSError:
            return
        self.endpoint = NetEndpoint(conn, self.on_move)

    def send_move(self, start, end):
        if self.endpoint:
            self.endpoint.send_move(start, end)

    def close(self):
        if self.endpoint:
            self.endpoint.close()
        try:
            self.sock.close()
        except:
            pass

class GameClient:
    def __init__(self, host, port=5000, on_move=None):
        self.on_move = on_move
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((host, port))
        self.endpoint = NetEndpoint(self.sock, self.on_move)

    def send_move(self, start, end):
        self.endpoint.send_move(start, end)

    def close(self):
        if self.endpoint:
            self.endpoint.close()
        try:
            self.sock.close()
        except:
            pass
