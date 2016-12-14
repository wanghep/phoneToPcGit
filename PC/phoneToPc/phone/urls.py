from django.conf.urls import patterns,url
from phone import views


urlpatterns=patterns('',
    url(r'^codescan/$',views.code_scan ,name = 'code_scan'),
    url(r'^releaseConnect/$',views.releaseConnect ,name = 'releaseConnect'),


    )