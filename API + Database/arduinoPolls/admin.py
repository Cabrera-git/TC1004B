from django.contrib import admin
from .models import *

# Register your models here.

class ArduinoAdmin(admin.ModelAdmin):
    list_display = ('__str__', 'Creado', 'Modificado')

class LecturasAdmin(admin.ModelAdmin):
    list_display = ('Tiempo_lectura', 'nombre_arduino', 'Lectura_1', 'Lectura_2', 'Lectura_3')
    list_filter = ('Tiempo_lectura', 'Arduino')

admin.site.register(Arduino, ArduinoAdmin)
admin.site.register(Lecturas, LecturasAdmin)