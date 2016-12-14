/**
 * Created by wanghp1 on 2015/2/27.
 */

var runningSession = 0; 
var connectSuccess = 0;
var phoneInputEnable = 1;
var phoneIpAddress="";
var source;
var actionSwitch = 0;

var canvas;
var context = null; // global variable 2d context
var x = 0, y =0;
var count;
//var image;

window.onload = function()
{
	count=0;

        canvas = document.getElementById("myCanvas");
	if (!canvas.getContext) 
        {
	    return;
	}
	context = canvas.getContext("2d");
	// mouse event 
	canvas.addEventListener("click", mouseClick, false);

	image = new Image();
	refreshPic();
}

function refreshPic() 
{
	var timeout;
	if( connectSuccess == 1 && (phoneInputEnable)  )
    	{	
		convertImageToCanvas('http://'+phoneIpAddress + ':8080/'+count+'.jpeg?t='+Math.random());
		//loadImage('http://'+phoneIpAddress + ':8080/'+count+'.jpeg?t='+Math.random());
		/*count++;
		if(count>9)
		{
	       	    count = 0;
       		}
		*/
	}
	else
	{
		//终止定时器
		clearTimeout(timeout);
		convertImageToCanvas("static/img/test.png");
	}
	timeout = setTimeout("refreshPic()", 2000);

}


//直接使用img标签加载
function loadImage(url) 
{
	var img = document.getElementById("img"); 
	img.src = url;
}
		

function convertImageToCanvas(url)
{
    var image = new Image();
    image.onload = function()  //onload: 事件会在图像加载完成后立即发生
    {  	     
	if (image.width != canvas.width) {

		canvas.width = image.width;
	    }

        if (image.height != canvas.height) {

		canvas.height = image.height;
	    }
	
        // canvas清屏
	//x.clearRect(0, 0, canvas.width, canvas.height);
 	context.drawImage(image, 0, 0, 360, 640);
     }
    // 设置src属性，浏览器会自动加载。
    // 记住必须先绑定事件，才能设置src属性，否则会出同步问题。
    image.src = url;
    //delete image;
}

function mouseClick(evt) 
{ 
   var mousePos = getPointOnCanvas(canvas, evt);
   var x=parseInt(mousePos.x);
   var y=parseInt(mousePos.y);
   console.log('input_window:' , evt.clientX +','+ evt.clientY);
   console.log('input_canvas:' , mousePos.x +','+ mousePos.y); 
   console.log('input_canvas_parseInt:' , x +','+ y); 
   //PC to Phone click 
   var X=x*4;
   var Y=y*4;
   if( connectSuccess == 1 && (phoneInputEnable)  )
   {

        var message = {mouseX : X , mouseY : Y};

        myPost(message);

   }
   //alert(x +',' +y); 
}

//Get Mouse Position 
function getPointOnCanvas(canvas, evt) 
{ 
   var rect = canvas.getBoundingClientRect(); 
   return { 
     x: evt.clientX - rect.left * (canvas.width / rect.width),
     y: evt.clientY - rect.top * (canvas.height / rect.height)
   }
}


function init()
{
    window.onbeforeunload = onbeforeunload_handler;
    //window.onunload = onunload_handler;
    document.onkeydown = oncatchKeyDown;
    document.onkeyup = oncatchKeyUp;
    //document.onmousemove = oncatchMouseMove;
    //document.onmousedown = oncatchMouseDown;
    //doucument.onmouseup = oncatchMouseUp;
    connectSuccess = 0;
    phoneInputEnable = 1;

}
function saveqcodeContext( qcode_context )
{
    console.log("qcode_context:" , qcode_context );
    
    var re = /sessionId=(\d+$)/;
    var arr = re.exec(qcode_context);

    if( arr.length > 0 ) {
        console.log("re.exec result:", arr[1]);
        runningSession = arr[1];
    }             
}

function connectStateEventTriger() {
    source = new EventSource("/pc/eventsource/?sessionId="+runningSession);
    source.onmessage = connectStateEventHandle;

}


function connectStateEventHandle( event )
{
    console.log("connectStateEventHandle event.data:" , event.data );
    
    var jsonData = JSON.parse(event.data)
    console.log("jsonData:" , jsonData );

    if(jsonData.status == 0)
    {
    	
        /* 显示视频内容*/
        //var videoDiv = document.getElementById("videoDiv")
        //videoDiv.style.display = "block";
        //videoDiv.style.visibility ="visible";
	
        /* 显示canvas*/
        var canvasDiv = document.getElementById("canvasDiv")
        canvasDiv.style.display = "block";
        canvasDiv.style.visibility ="visible";


        /* 二维码消失 */
        var qrcode = document.getElementById("qrcode");
        qrcode.innerHTML = "连接成功";
        qrcode.style.fontSize = "30pt";
        qrcode.style.color = "red"
        qrcode.style.weight = "bold";


        /* 解析phone的IP地址 */
        phoneIpAddress = jsonData.phoneIpAddress;

        /* 设置连接状态*/
        connectSuccess = 1;


    }
}


function Run(strPath) {
    try {
        var objShell = new ActiveXObject("wscript.shell");
        objShell.Run(strPath);
        objShell = null;
    }
    catch (e) {
        alert('找不到文件"' + strPath + '请确定路径和文件名是否正确.')
    }
}



function onbeforeunload_handler(){
    source.close();
    console.log( "onbeforeunload_handler" );
    source = null;
}

function onunload_handler(){
    var warning="谢谢光临";
    alert(warning);
}
function oncatchKeyDown(event){
    var e = event || window.event || arguments.callee.caller.arguments[0];
    console.log('event.keyCode:',event.keyCode);
    console.log('event:',event);
	  
    actionSwitch = 1;

    if(e && e.keyCode==113){ // 按 F2
        phoneInputEnable = !phoneInputEnable;
    }
    if(e && e.keyCode==13){ // enter 键
        //要做的事情
    }

    if( connectSuccess == 1 && (phoneInputEnable)  )
    {

        var message = {keyCode : e.keyCode , keyAction : actionSwitch};

        myPost(message);

        if( e.keyCode== 8){ // 按 backspace
            var ev = e || window.event;//获取event对象
            var obj = ev.target || ev.srcElement;//获取事件源
            var t = obj.type || obj.getAttribute('type');//获取事件源类型
            //获取作为判断条件的事件类型
            var vReadOnly = obj.readOnly;
            var vDisabled = obj.disabled;
            //处理undefined值情况
            vReadOnly = (vReadOnly == undefined) ? false : vReadOnly;
            vDisabled = (vDisabled == undefined) ? true : vDisabled;
            //当敲Backspace键时，事件源类型为密码或单行、多行文本的，
            //并且readOnly属性为true或disabled属性为true的，则退格键失效
            var flag1 = ev.keyCode == 8 && (t == "password" || t == "text" || t == "textarea") && (vReadOnly == true || vDisabled == true);
            //当敲Backspace键时，事件源类型非密码或单行、多行文本的，则退格键失效
            var flag2 = ev.keyCode == 8 && t != "password" && t != "text" && t != "textarea";
            //判断
            if (flag2 || flag1) return false


            return false;
        }
    }


    /*
    $.post('http://10.106.10.64:8080/hci', {keyCode : e.keyCode,keyCode:e.keyCode},function(re) {

            console.log('post.reponse:',re );

    }
    );
    */
};

function oncatchKeyUp(event){
    var e = event || window.event || arguments.callee.caller.arguments[0];
    console.log('event.keyCode:',event.keyCode);
    console.log('event:',event);
    
    actionSwitch = 0;

    if( connectSuccess == 1 && (phoneInputEnable)  )
    {

        var message = {keyCode : e.keyCode , keyAction : actionSwitch};

        myPost(message);

        if( e.keyCode== 8){ // 按 backspace
            var ev = e || window.event;//获取event对象
            var obj = ev.target || ev.srcElement;//获取事件源
            var t = obj.type || obj.getAttribute('type');//获取事件源类型
            //获取作为判断条件的事件类型
            var vReadOnly = obj.readOnly;
            var vDisabled = obj.disabled;
            //处理undefined值情况
            vReadOnly = (vReadOnly == undefined) ? false : vReadOnly;
            vDisabled = (vDisabled == undefined) ? true : vDisabled;
            //当敲Backspace键时，事件源类型为密码或单行、多行文本的，
            //并且readOnly属性为true或disabled属性为true的，则退格键失效
            var flag1 = ev.keyCode == 8 && (t == "password" || t == "text" || t == "textarea") && (vReadOnly == true || vDisabled == true);
            //当敲Backspace键时，事件源类型非密码或单行、多行文本的，则退格键失效
            var flag2 = ev.keyCode == 8 && t != "password" && t != "text" && t != "textarea";
            //判断
            if (flag2 || flag1) return false


            return false;
        }
    }


    /*
    $.post('http://10.106.10.64:8080/hci', {keyCode : e.keyCode,keyCode:e.keyCode},function(re) {

            console.log('post.reponse:',re );

    }
    );
    */
};

function oncatchMouseMove(event){
    var e = event || window.event || arguments.callee.caller.arguments[0];
    console.log('event.screenX/y:',event.screenX ,event.screenY );


    if( connectSuccess == 1 && (phoneInputEnable)  )
    {
        var message = {screen_coord :[event.screenX,event.screenY] };

        myPost(message);
    }

};

function oncatchMouseDown(event){

};

function oncatchMouseUp(event){


};

function myPost( myContext )
{
    $.post('http://'+phoneIpAddress + ':8080/hci', myContext , function(re) {
       
	//console.log('myPost.reponse ',this.getServletContext().getRealPath(”/”));
	console.log('myPost.reponse ',myContext);
	console.log('post.reponse ',Object.prototype.toString.apply(re));
        //console.log('myPost.reponse ',re);
        //console.log('mypost.reponse ','http://'+phoneIpAddress + ':8080/hci' );

    });
};

function refreshPost( myContext )
{
    $.post('http://'+phoneIpAddress + ':8080/hci', myContext , function(re) {

        console.log('refreshPost.reponse ',myContext);
	console.log('post.reponse ',Object.prototype.toString.apply(re));
        //console.log('refreshPost.reponse ',re);
        //console.log('refreshPost.reponse ','http://'+phoneIpAddress + ':8080/hci' );
        convertImageToCanvas('http://'+phoneIpAddress + ':8080/');
    });
};


