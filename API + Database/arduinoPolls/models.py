from django.db import models
from django.db.models.base import Model
from django.db.models.fields.related import OneToOneField
from .randomNames import makeRandomName
import uuid
import datetime

# Create your models here.
class Arduino(models.Model):
    id = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False, null=False, unique=True)
    Nombre = models.CharField(max_length= 50, default=makeRandomName, editable=True)
    Creado = models.DateTimeField(default=datetime.datetime.now, editable=False, null=False)
    Modificado = models.DateTimeField(editable=True, null=True, blank=True)
    
    def __str__(self) -> str:
        return f"{self.Nombre} ({self.id})"

    def getNewArduino() -> str:
        new = Arduino.objects.create()
        return f"{new.id}, {new.Nombre}"

class Lecturas(models.Model):
    Arduino = models.ForeignKey(Arduino, unique=False, on_delete=models.CASCADE)
    Tiempo_lectura = models.DateTimeField(default=datetime.datetime.now, editable=False, null=False)
    Lectura_1 = models.FloatField(null = True, help_text="CO2")
    Lectura_2 = models.FloatField(null = True, help_text="CO")
    Lectura_3 = models.FloatField(null = True, help_text="Gases contaminantes")

    def __str__(self) -> str:
        return self.Arduino.Nombre

    def nombre_arduino(self) -> str:
        return self.Arduino.Nombre