class Drone():
    def __init__(self, nome, bateria, posicao):
        raise NotImplementedError

    def takeoff(self, altura):
        raise NotImplementedError

    def set_position(self, x, y):
        raise NotImplementedError

    def land(self):
        raise NotImplementedError

    def mapear(self):
        raise NotImplementedError

    def status(self):
        raise NotImplementedError
