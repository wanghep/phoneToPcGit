from django.conf.urls import patterns,url
from pc import views

from django.conf import settings

urlpatterns=patterns('',
	   url(r'^eventsource/$', views.eventsource , name = 'eventsource'),
       url(r'^query/$',views.queryPhoneScanResult ,name = 'queryPhoneScanResult'),      
       #url(r'^releaseConnect/$',phone.views.releaseConnect ),     
       url(r'^static/(?P<path>.*)$','django.views.static.serve',{'document_root':settings.STATIC_ROOT},name='static'),
       url('^',views.mainSite ,name = 'mainSite'),
          
    )