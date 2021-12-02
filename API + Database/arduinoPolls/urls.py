from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('<UUID>/<val1>/<val2>/<val3>/', views.input, name='Info Input'),
    path('register/', views.generateUser, name='regis'),
]