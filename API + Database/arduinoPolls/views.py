from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render
from django.http import HttpResponse
from .models import *
import datetime

# Default view
def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")

@csrf_exempt
def generateUser(request):
    if request.method == 'POST':
        thingy = Arduino.getNewArduino()
        return HttpResponse(thingy)
    else:
        return render(request, '1.html')

@csrf_exempt
def input(request, UUID, val1, val2, val3):
    try:
        elegido = Arduino.objects.get(id = UUID)
        Lecturas.objects.create(Arduino = elegido, Lectura_1 = val1, Lectura_2 = val2, Lectura_3 = val3)
        elegido.Modificado = datetime.datetime.now()
        elegido.save()
        return HttpResponse("Suxess")
    except Exception as e:
        return HttpResponse(f"Error: {e}")