from django.db import models
from django.contrib import admin
# Create your models here.
class pcUserModel(models.Model):
    sessionId = models.IntegerField(default='0')
    phoneIpAddress=models.GenericIPAddressField(default='0.0.0.0',blank=False,null=False)
    phoneHandshakeCode = models.IntegerField(default='0')
    reverse = models.IntegerField(default='0')
    def __str__(self):
        return str(self.sessionId)

class globalVariableModel(models.Model):
	constIndex = models.IntegerField(default='0')
	sessionId = models.IntegerField(default='0')
	
	def __str__(self):
		return str(self.sessionId)
