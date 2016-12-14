# -*- coding: utf-8 -*-
import time
from django.utils.timezone import now
from django.shortcuts import render
import logging
import json
from pc.models import pcUserModel 
from pc.models import globalVariableModel
from django.core.exceptions import ObjectDoesNotExist
from django.template.loader import get_template
from django.template import Context
from django.http import HttpResponse
from django.http import HttpResponseNotFound
from django.shortcuts import render_to_response
from django.http import StreamingHttpResponse

# Create your views here.
logger = logging.getLogger(__name__)
#import sys
#reload(sys)
#sys.setdefaultencoding('utf-8')


staticWebAddress = "http://phoneToPc"

def mainSite( request ):        
    logger.debug("mainSite")
    if request.method=='GET':  
        #if request.method == 'POST':
        logger.debug(str(request.GET))
        
        #t = get_template('main.html')
        
        #html = render(t,{"qcode_context":g_qcode_context} )
        #return HttpResponse(html)
		
        globleObj=None
        try:
            globleObj = globalVariableModel.objects.get( constIndex = 0 )  
        except ObjectDoesNotExist:
            globleObj = globalVariableModel(constIndex=0,sessionId=0)
 			
        g_sessionIdAuto = globleObj.sessionId  
        globleObj.sessionId  += 1
        globleObj.save()
        
        
        m_pcUserModel = pcUserModel(sessionId=g_sessionIdAuto,phoneIpAddress='0.0.0.0',phoneHandshakeCode = 0 , reverse = 0 )
        m_pcUserModel.save()
        
        qcode_context = staticWebAddress + "?/sessionId=" + str(g_sessionIdAuto);
        return render_to_response ('main.html',{"qcode_context":qcode_context})
    
    return HttpResponseNotFound


def eventsource(request):
    logger.debug("eventsource enter")
    data = {}
    data['status']=1
    data['error']='missing items'
    response = StreamingHttpResponse(queryPhoneScanResult(request), content_type="text/event-stream")
    response['Cache-Control'] = 'no-cache'
    logger.debug("eventsource return ")
    return response

def queryPhoneScanResult( request ):
    data = {}
    logger.debug("queryPhoneScanResult")
        
    if request.method=='GET':        
        logger.debug(str(request.GET))

        try:
            m_sessionId=request.GET.get('sessionId')

        except KeyError:
            data['status']=1
            data['error']='missing items'
            #return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
            logger.debug("queryPhoneScanResult break1")
            return u'data: %s\n\n' % str(json.dumps(data,ensure_ascii=False))
        #save it to databse
        try:            
            sessionObj = pcUserModel.objects.get(sessionId =m_sessionId )
        except ObjectDoesNotExist:
            data['status']=2
            data['error']='no this session'
            #return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
                  
            logger.debug("queryPhoneScanResult break2")
            return u'data: %s\n\n' % str(json.dumps(data,ensure_ascii=False))
         
        if sessionObj.phoneIpAddress == '0.0.0.0':
            data['status']=3
            data['error']='no phone try to connect'
            
            logger.debug("queryPhoneScanResult break3")
            return( u'data: %s\n\n' % str(json.dumps(data,ensure_ascii=False)))
            
         
        #success
        data['status']=0 
        data['phoneIpAddress']= sessionObj.phoneIpAddress
        data['phoneHandshakeCode']= sessionObj.phoneHandshakeCode        
        #return HttpResponse(json.dumps(data,ensure_ascii=False),content_type='application/json')
        logger.debug("queryPhoneScanResult break4")
        return( u'data: %s\n\n' % str(json.dumps(data,ensure_ascii=False)))
            
    logger.debug("queryPhoneScanResult break last")
    data['status']=404
    return u'data: %s\n\n' % str(json.dumps(data,ensure_ascii=False))
                
    
