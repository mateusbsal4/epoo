class Bateria():
    def __init__(self, mah, tempoDeCarregamento):
        raise NotImplementedError

    def carregar(self, tempo):
        raise NotImplementedError

    def usar(self, tempo):
        raise NotImplementedError

    def calcula_tempo_de_voo(self):
        raise NotImplementedError

    def status(self):
        raise NotImplementedError
