from django.shortcuts import render
import logging
import json
from pc.models import pcUserModel 
from django.core.exceptions import ObjectDoesNotExist

from django.http import HttpResponse

#from django.views.decorators.csrf import csrf_exempt
#@csrf_exempt

# Create your views here.
logger = logging.getLogger(__name__)

def code_scan(request):
    data = {}
    logger.debug(str(request.POST))
    logger.debug(request)
    
    if request.method=='POST':        
       

        try:
            m_sessionId=request.POST.get('sessionId')
            m_ipAddress=request.POST.get('IpAddress')
            m_phoneHandshakeCode=request.POST.get('phoneHandshakeCode')
        
        except KeyError:
            data['status']=1
            data['error']='missing items'
            return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json') 
        #save it to databse
        try:            
            sessionObj = pcUserModel.objects.get(sessionId =m_sessionId )
        except ObjectDoesNotExist:
            data['status']=2
            data['error']='no this session'
            return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
         
        sessionObj.phoneIpAddress = m_ipAddress
        sessionObj.phoneHandshakeCode = m_phoneHandshakeCode
        #import pdb
        #pdb.set_trace()
        
        sessionObj.reverse = 0
        sessionObj.save()
        
        #success
        data['status']=0 
        return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
    
    data['status']=404
    return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')    
    
def releaseConnect( request ):
    
    data = {}
    logger.debug(str(request.POST))
    
    if request.method == 'POST':
        
    
        try:
            m_sessionId=request.POST.get('sessionId')                
        except KeyError:
            data['status']=1
            data['error']='missing items'
            return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json') 
        #erase phone address record
        try:            
            sessionObj = pcUserModel.objects.get(sessionId =m_sessionId )
        except ObjectDoesNotExist:
            data['status']=2
            data['error']='no this session'
            return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
         
        sessionObj.delete()   #delete
        sessionObj.save()
    
    #    success
        data['status']=0 
        return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
    
    data['status']=404
    return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')    

         