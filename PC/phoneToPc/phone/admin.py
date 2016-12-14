from django.contrib import admin
from django.contrib import admin
from pc.models import pcUserModel 
from pc.models import globalVariableModel
# Register your models here.

admin.site.register(pcUserModel)
admin.site.register(globalVariableModel)
