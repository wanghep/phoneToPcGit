from django.conf.urls import patterns, include, url
from django.contrib import admin
from argparse import Namespace


urlpatterns = patterns('',
    # Examples:
    url(r'^$', include('pc.urls',namespace='pc')),
    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
    url(r'^phone/', include('phone.urls',namespace='phone')),
    url(r'pc/',include('pc.urls',namespace='pc')),
)
