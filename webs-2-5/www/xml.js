
function addRow() {
	if (addRow.rows == undefined) {
		addRow.rows = tab.rows.length;
	} else {
		addRow.rows++;
	}
	var newTr = tab.insertRow(-1);
	var fieldName = 'field' + addRow.rows;
	var fieldValue = 'value' + addRow.rows;
	var coordXName = 'x' + addRow.rows;
	var coordYName = 'y' + addRow.rows;
	var coordX = (addRow.rows % 2) * 400;
	var coordY = (addRow.rows / 2) * 40;

	var newTd = newTr.insertCell(-1);
	newTd.innerHTML = '<label>' + addRow.rows + '</label>';
	var newTd = newTr.insertCell(-1);
	//newTd.innerHTML='<input type="text" name='+fieldName+' id='+fieldName+' value='+fieldName+'>';
	newTd.innerHTML = '<select name="" id=""><option value="text">文本</option><option value="date">日期</option><option value="time">时间</option><option value="count">计数器</option><option value="picture">图片</option></select>'

		var newTd = newTr.insertCell(-1);
	newTd.innerHTML = '<input type="text" name=x value=' + coordX + '>';
	var newTd = newTr.insertCell(-1);
	newTd.innerHTML = '<input type="text" name=y value=' + coordY + '>';

	var newTd = newTr.insertCell(-1);
	newTd.innerHTML = '<input type="text" name=' + fieldValue + ' id=' + fieldValue + ' value=' + fieldValue + '>';

	var newTd = newTr.insertCell(-1);
	// newTd7.innerHTML='<input type="button" value="Ìí¼Ó×Ö¶Î" name="btn1" id="btn1">';
	newTd.innerHTML = '<input type="button" value="删除字段" name="btn1" id="btn1" onClick="delRow(this)">';

}
function delRow(btn) {
	if (tab.rows.length > 2) {
		var row = btn.parentNode.parentNode;
		row.parentNode.removeChild(row);
	}
}

function doXML(del) {
	if (del == 0) {
		var xmlDocument = '<?xml version="1.0" encoding="UTF-8" ?><Production>';
		var httpRequest;
		var table = document.getElementById("tab");
		var rows = table.rows;
		for (var i = 1; i < rows.length; ++i) {
			var cells = rows[i].cells;
			xmlDocument += '<' + cells[1].childNodes[0].value;
			for (var j = 2; j < cells.length - 2; j++) {
				xmlDocument += ' ' + cells[j].childNodes[0].name + '=' + cells[j].childNodes[0].value;
				//alert("name=" +cells[j].childNodes[0].name +" value="+ cells[j].childNodes[0].value);
			}
			xmlDocument += '>' + cells[j].childNodes[0].value + '</' + cells[1].childNodes[0].value + '>';
		}
		// 	xmlDocument += fill_xml_helper(defaultRows[i],table.rows[i]);

		xmlDocument += "</Production>";

	}
	//alert(xmlDocument)
	if (window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
		httpRequest = new XMLHttpRequest();
	} else { // code for IE6, IE5
		httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
	}
	if (del == 0) {
		httpRequest.open('POST', "/cgi-bin/xml?action=save&file=" + document.getElementById("file").value + '.xml', false);
		httpRequest.send(xmlDocument);
	} else {
		httpRequest.open('GET', "/cgi-bin/xml?action=delete&file=" + document.getElementById("file").value + '.xml', false);
		httpRequest.send();
	}
	window.location.href = "/list.html";

}

////////////////////////////////////////////////////////////////////////////////////////////
//   list action
//////////////////////////////////////////////////////////////////////////

function getFiles() {
	var selected = document.getElementsByName("file");
	var value = "";
	for (var i = 0; i < selected.length; i++) {
		if (selected[i].checked == true) {
			value += selected[i].value + "|";
		}
	}
	return value;
}
function doAction(act) {
	var value = getFiles();
	if (value == "") {
		alert("必须选择一个");
	} else {
		// httpRequest = new XMLHttpRequest();
		// httpRequest.open('GET', "xml?action="+act+"&file="+value, false);
		// httpRequest.send();
		window.location.href = "/cgi-bin/xml?action=" + act + "&file=" + value;
	}

}

function doActionDel(act) {
	var value = getFiles();
	if (value == "") {
		alert("必须选择一个");
	} else {
		if (window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
			httpRequest = new XMLHttpRequest();
		} else { // code for IE6, IE5
			httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
		}
		httpRequest.open('GET', "/cgi-bin/xml?action=" + act + "&file=" + value, false);
		httpRequest.send();
		window.location.href = "/list.html";
	}
}

function doActionPrint(act) {
	var value = getFiles();
	if (value == "") {
		alert("必须选择一个");
	} else {
		if (window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
			httpRequest = new XMLHttpRequest();
		} else { // code for IE6, IE5
			httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
		}
		httpRequest.open('GET', "/cgi-bin/xml?action=" + act + "&file=" + value, false);
		httpRequest.send();
		var lines = httpRequest.responseText.split("\n");
		//alert(lines[lines.length-1]);
		window.open(lines[lines.length-1],"blank");
		//window.location.href = httpRequest.responseText;
	}
}
