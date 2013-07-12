<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title>主页</title>
</head>

<body>
<%write("yyy");%>
	<form action="cgi-bin/test" method="get">
		<p align="center">
			<input name="textFilename" type="text" value="" />
		</p>
		<p align="center">
			<input name="preview" type="image" value="预览" src="preview.jpg" />
		</p>
		<p align="center">
			<input name="bPrint" type="button" value="打印" onclick="window.location.href='print.html'"/>
			<input name="bManage" type="button" value="管理" />
			<input name="bModify" type="button" value="编辑" onclick="window.location.href='modify.html'"/>
		</p>
	</form>
</body>
</html>


