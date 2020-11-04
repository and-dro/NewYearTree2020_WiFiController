#include <WiFiClient.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Гирлянда 2020+</title>
  <style>
    * {margin:0;}
    html {font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    color:#515151;
    display: inline-block; text-align: left;background-color: #fff;padding: 0px;}
    h2 {font-size: 2.4rem;text-align:center;margin-bottom: 1ex;}
    p {font-size: 2.2rem;margin-left: 5%;padding-bottom: 2ex;}
    body {max-width: 600px; margin:10px auto;  padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .control {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .control:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; 
      background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.control {background-color: #2196F3}
    input:checked+.control:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .text {margin-left: 140px;display:block;width: 320px;padding-top: 5px;}
    .slider { -webkit-appearance: none; margin: 4px 0px; width: 90%; height: 52px; background: #ccc;
      outline: none; -webkit-transition: .5s; transition: opacity .5s;border-radius: 26px;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 52px; height: 52px; background: #2f4468; cursor: pointer;border-radius: 26px;}
    .slider::-moz-range-thumb { width: 52px; height: 52px; background: #2f4468; cursor: pointer; } 
    .speed::-webkit-slider-thumb{ background-repeat:no-repeat;background-image: url("data:image/svg+xml;utf8;base64,PHN2ZyB3aWR0aD0iNTIiIGhlaWdodD0iNTIiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+CiA8Zz4KICA8cGF0aCBmaWxsPSIjMjE5NkYzIiBkPSJtMjYsMTZjLTUuNDg4MjksMCAtOS45Mzc0OSw0LjQ3NzE5IC05LjkzNzQ5LDEwYzAsNS41MjI3MyA0LjQ0OTIsMTAgOS45Mzc0OSwxMGM1LjQ4ODIxLDAgOS45Mzc0OSwtNC40NzcyNyA5LjkzNzQ5LC0xMGMwLC01LjUyMjgxIC00LjQ0OTI4LC0xMCAtOS45Mzc0OSwtMTB6bTAsMTYuMjVsMCwtMTIuNWMzLjQyNDYzLDAgNi4yMTA5MywyLjgwMzc1IDYuMjEwOTMsNi4yNWMwLDMuNDQ2MTcgLTIuNzg2Myw2LjI1IC02LjIxMDkzLDYuMjV6bTAsNi4yNWMxLjM3MjA3LDAgMi40ODQzNywxLjExOTMgMi40ODQzNywyLjVsMCwyLjVjMCwxLjM4MDcgLTEuMTEyMywyLjUgLTIuNDg0MzcsMi41cy0yLjQ4NDM3LC0xLjExOTMgLTIuNDg0MzcsLTIuNWwwLC0yLjVjMCwtMS4zODA3IDEuMTEyMywtMi41IDIuNDg0MzcsLTIuNXptMCwtMjVjLTEuMzcyMDcsMCAtMi40ODQzNywtMS4xMTkzIC0yLjQ4NDM3LC0yLjVsMCwtMi41YzAsLTEuMzgwNyAxLjExMjMsLTIuNSAyLjQ4NDM3LC0yLjVzMi40ODQzNywxLjExOTMgMi40ODQzNywyLjVsMCwyLjVjMCwxLjM4MDcgLTEuMTEyMywyLjUgLTIuNDg0MzcsMi41em0xNy4zOTA2MSwxMGMxLjM3MjE1LDAgMi40ODQzNywxLjExOTMgMi40ODQzNywyLjVzLTEuMTEyMjIsMi41IC0yLjQ4NDM3LDIuNWwtMi40ODQzNywwYy0xLjM3MjE1LDAgLTIuNDg0MzcsLTEuMTE5MyAtMi40ODQzNywtMi41czEuMTEyMjIsLTIuNSAyLjQ4NDM3LC0yLjVsMi40ODQzNywwem0tMjkuODEyNDgsMi41YzAsMS4zODA3IC0xLjExMjMsMi41IC0yLjQ4NDM3LDIuNWwtMi40ODQzNywwYy0xLjM3MjA3LDAgLTIuNDg0MzcsLTEuMTE5MyAtMi40ODQzNywtMi41czEuMTEyMywtMi41IDIuNDg0MzcsLTIuNWwyLjQ4NDM3LDBjMS4zNzIwNywwIDIuNDg0MzcsMS4xMTkzIDIuNDg0MzcsMi41em0yNC43MTg5Nyw4LjgzODgzbDEuNzU2NjgsMS43Njc4OWMwLjk3MDE1LDAuOTc2MTcgMC45NzAxNSwyLjU1OTMgMCwzLjUzNTQ3Yy0wLjk3MDE1LDAuOTc2MjUgLTIuNTQzMjIsMC45NzYyNSAtMy41MTMzNywwbC0xLjc1Njg0LC0xLjc2NzczYy0wLjk3MDE1LC0wLjk3NjI1IC0wLjk3MDE1LC0yLjU1OTMgMCwtMy41MzU2M2MwLjk3MDMsLTAuOTc2MjUgMi41NDMzLC0wLjk3NjI1IDMuNTEzNTMsMHptLTI0LjU5NDEzLC0xNy42Nzc2NmwtMS43NTY3NiwtMS43Njc4MWMtMC45NzAyMywtMC45NzYzMyAtMC45NzAyMywtMi41NTkyMiAwLC0zLjUzNTU1czIuNTQzMjIsLTAuOTc2MzMgMy41MTM0NSwwbDEuNzU2NzYsMS43Njc4MWMwLjk3MDIzLDAuOTc2MzMgMC45NzAyMywyLjU1OTIyIDAsMy41MzU1NWMtMC45NzAyMywwLjk3NjMzIC0yLjU0MzIyLDAuOTc2MzMgLTMuNTEzNDUsMHptMjQuNTk0MTMsMGMtMC45NzAxNSwwLjk3NjI1IC0yLjU0MzIyLDAuOTc2MjUgLTMuNTEzNTMsMGMtMC45NzAxNSwtMC45NzYzMyAtMC45NzAxNSwtMi41NTkyMiAwLC0zLjUzNTU1bDEuNzU2ODQsLTEuNzY3NzNjMC45NzAxNSwtMC45NzYzMyAyLjU0MzIyLC0wLjk3NjMzIDMuNTEzMzcsMHMwLjk3MDE1LDIuNTU5MjIgMCwzLjUzNTU1bC0xLjc1NjY4LDEuNzY3NzN6bS0yNC41OTQxMywxNy42Nzc2NmMwLjk3MDE1LC0wLjk3NjI1IDIuNTQzMjIsLTAuOTc2MjUgMy41MTMzNywwYzAuOTcwMjMsMC45NzYzMyAwLjk3MDIzLDIuNTU5MzggMCwzLjUzNTYzbC0xLjc1NjY4LDEuNzY3NzNjLTAuOTcwMjMsMC45NzYyNSAtMi41NDMyMiwwLjk3NjI1IC0zLjUxMzQ1LDBjLTAuOTcwMjMsLTAuOTc2MTcgLTAuOTcwMjMsLTIuNTU5MyAwLC0zLjUzNTQ3bDEuNzU2NzYsLTEuNzY3ODl6Ii8+CiA8L2c+Cjwvc3ZnPg==")}
    .brigtness::-webkit-slider-thumb{ background-repeat:no-repeat;background-image: url("data:image/svg+xml;utf8;base64,PHN2ZyB3aWR0aD0iNTIiIGhlaWdodD0iNTIiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+CiAgPGc+CiAgIDxwYXRoIGZpbGw9IiMyMTk2RjMiIGQ9Im0yNiwzMS45OTkzNWMzLjQ3ODEsMCA2LjI4NTcxLC0yLjg1NDYxIDYuMjg1NzEsLTYuMzkwOTJjMCwtMi4zODU5NCAtMS4yNzgxLC00LjQ3MzY0IC0zLjE0Mjg2LC01LjU2MDFsLTIwLjM0NDc2LC0xMS45NzIzMmwxMS41ODY2NywyMC40MDgzM2MxLjA0NzYyLDIuMDg3NyAzLjE2MzgxLDMuNTE1IDUuNjE1MjQsMy41MTVtMCwtMjcuNjkzOThjLTMuNzkyMzgsMCAtNy4zMzMzMywxLjA2NTE1IC0xMC40MTMzMywyLjgxMmw0LjQsMi41Nzc2N2MxLjgyMjg2LC0wLjcyNDMgMy45MTgxLC0xLjEyOTA2IDYuMDEzMzMsLTEuMTI5MDZjOS4yNjA5NSwwIDE2Ljc2MTkxLDcuNjI2NDkgMTYuNzYxOTEsMTcuMDQyNDVjMCw0LjcwNzk4IC0xLjg2NDc2LDguOTY4NTkgLTQuOTAyODYsMTIuMDM2MjNsLTAuMDIwOTUsMGMtMC44MTcxNCwwLjgzMDgyIC0wLjgxNzE0LDIuMTcyOTEgMCwzLjAwMzczYzAuODE3MTQsMC44MzA4MiAyLjE1ODEsMC44MzA4MiAyLjk3NTI0LDAuMDIxM2MzLjc5MjM4LC0zLjg1NTg1IDYuMTM5MDUsLTkuMTgxNjIgNi4xMzkwNSwtMTUuMDYxMjZjMCwtMTEuNzE2NjggLTkuNDI4NTcsLTIxLjMwMzA2IC0yMC45NTIzOCwtMjEuMzAzMDZtLTIwLjk1MjM4LDIxLjMwMzA2YzAsNS44Nzk2NCAyLjM0NjY3LDExLjIwNTQxIDYuMTM5MDUsMTUuMDYxMjZjMC44MTcxNCwwLjgwOTUyIDIuMTM3MTQsMC44MDk1MiAyLjk1NDI5LC0wLjAyMTNjMC44MTcxNCwtMC44MzA4MiAwLjgxNzE0LC0yLjE3MjkxIDAsLTMuMDAzNzNjLTMuMDM4MSwtMy4wODg5NCAtNC45MDI4NiwtNy4zMjgyNSAtNC45MDI4NiwtMTIuMDM2MjNjMCwtMi4xMzAzMSAwLjM5ODEsLTQuMjYwNjEgMS4xMzE0MywtNi4xNzc4OWwtMi41MzUyNCwtNC40NzM2NGMtMS43MzkwNSwzLjE5NTQ2IC0yLjc4NjY3LDYuNzc0MzcgLTIuNzg2NjcsMTAuNjUxNTN6Ii8+CiAgPC9nPgo8L3N2Zz4=")}
    .change::-webkit-slider-thumb{ background-repeat:no-repeat;background-image: url("data:image/svg+xml;utf8;base64,PHN2ZyB3aWR0aD0iNTIiIGhlaWdodD0iNTIiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+CiA8Zz4KICA8cGF0aCBmaWxsPSIjMjE5NkYzIiBkPSJtMTUuNTMzNiwzNy42NDA5N2MtMy44MDQ5OSwtMi43MzkxNCAtNi4yMzk5NCwtNi45MzkxNyAtNi4yMzk5NCwtMTEuNjQ5NTRjMCwtNi45NjI5IDUuMzI4MTcsLTEyLjc5NjM0IDEyLjUyOTY0LC0xNC40NTk2MWwwLC0zLjg1MTgxYy05LjUzMTg0LDEuNzMwOTkgLTE2LjcwNjE5LDkuMjcxNjkgLTE2LjcwNjE5LDE4LjMxMTQxYzAsNS41ODYyNyAyLjc0MDk2LDEwLjU5NTU0IDcuMDgxNjksMTQuMDE5MTNjMy4zNTk4MywyLjgwMDQ3IDUuNDQ3OTYsLTAuOTM2OTUgMy4zMzQ4LC0yLjM2OTU4em0yNC4wOTE1LC0yNS44MDM1OWMtMi4xMzkzNywtMS43MjkxMyAtNS4yNzE4NiwxLjA3NDAxIC0zLjIxODMsMi40NjI2NGMzLjg0MDE1LDIuNzM4MzQgNi4yOTk4Myw2Ljk1ODM3IDYuMjk5ODMsMTEuNjkxNGMwLDYuOTYzOTcgLTUuMzI5MDcsMTIuNzk4NDcgLTEyLjUyOTk0LDE0LjQ2MDY3bDAsMy44NTI2MWM5LjUzMTg0LC0xLjczMjg2IDE2LjcwNjE5LC05LjI3MjQ5IDE2LjcwNjE5LC0xOC4zMTM1NWMwLC01LjY1OTg2IC0yLjgxNDI2LC0xMC43Mjc1MiAtNy4yNTc3OCwtMTQuMTUzNzhsMCwwLjAwMDAxem0tMTUuOTU2NTEsMi45MjI1OGw2Ljc5NzQzLC0zLjY0MjVjMC45ODg5NCwtMC41MzAwNyAwLjk5MTAyLC0xLjM5MzQzIDAuMDA1MzYsLTEuOTI3MjRsLTYuODYwMywtMy43MTU1NmMtMC45ODY4NSwtMC41MzMgLTEuNzgzMzEsLTAuMTMwNjUgLTEuNzc4NTQsMC45MDE0OWwwLjAzNjk1LDcuNDc1MTFjMC4wMDI5OCwxLjAzMTYxIDAuODA4OTcsMS40Mzg3NiAxLjc5OTEsMC45MDg2OWwwLDAuMDAwMDF6bTQuNzIxNTIsMjIuNDgzMThsLTYuODYxMTksMy43MTU1NmMtMC45ODY1NiwwLjUzMzggLTAuOTg0NDcsMS4zOTY5IDAuMDA1NjYsMS45MjcyNGw2Ljc5NjgzLDMuNjQyNzdjMC45ODg5NCwwLjUyOSAxLjc5NjcyLDAuMTIzMTkgMS44MDE3OCwtMC45MDk3NmwwLjAzNTc2LC03LjQ3NDg0YzAuMDAzNTgsLTEuMDMxMzQgLTAuNzk0MDcsLTEuNDMzOTYgLTEuNzc4ODQsLTAuOTAwOTZsMCwtMC4wMDAwMXoiLz4KIDwvZz4KPC9zdmc+")}
  </style>
</head>
<body onload="init()"><h2>Гирлянда 2020+</h2>
  <p><input type="range" onchange="sendSettings(this)" id="speed" min="0" max="255" value=0 step="1" class="slider brigtness"></p>
  <p><input type="range" onchange="sendSettings(this)" id="brightness" min="0" max="255" value=0 step="1" class="slider speed"></p>
  <p><input type="range"onchange="sendSettings(this)" id="change" min="0" max="255" value=0 step="1" class="slider change"></p>
  <p><label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="mode0" class="mode"/><span class="control"></span><span class="text">Выключено</span</label></p>
  <p><label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="mode1" class="mode"/><span class="control"></span><span class="text">Мерцание</span></label></p>
  <p><label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="mode2" class="mode"/><span class="control"></span><span class="text">Дождь</span></label></p>
  <p><label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="mode3" class="mode"/><span class="control"></span><span class="text">Полоски</span></label></p>
  <p><label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="mode4" class="mode"/><span class="control"></span><span class="text">Разметка</span></label></p>
<script>
var stateId;
function init(){
  stateId = 0;
  window.setInterval(updateState, 1000);
  updateState();
}

function sendSettings(element){
  postAll();
}
function toggleCheckbox(element) {
  var collection = document.getElementsByClassName("mode");
  for(var i=0;i<collection.length;i++)
  {
    collection[i].checked = collection[i].id == element.id;
  }
  postAll();
}
function gobj(id){return document.getElementById(id);}
function mode(){
  var collection = document.getElementsByClassName("mode");
  for(var i=0;i<collection.length;i++)
  {
    if (collection[i].checked) return collection[i].id.slice(4);
  }
  return 0;
}
function postAll() 
{
  if(stateId == 2){
      window.clearTimeout(postAll);
      window.setTimeout(postAll, 200);
  }
  stateId = 2;
  var state = "b="+gobj("brightness").value+"&s="+gobj("speed").value+"&c="+gobj("change").value+"&m="+mode();
  var xhr = new XMLHttpRequest();
  xhr.open("PUT", "/set?" +  state,  true); 
  xhr.onreadystatechange = function () {
        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
          stateId = 1;
        };
    };    
  xhr.send();
  window.setTimeout(sendTimeOut, 3000);
}
function sendTimeOut()
{
  stateId = 0;
}
function updateState() 
{
  if(stateId != 0) return;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/set",  true); 
  xhr.onreadystatechange = function () {
        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
          showState(xhr.responseText);
        };
    };    
    xhr.send();
}
function showState(responseText) 
{
  if(stateId > 0)
  {
    stateId--;
    return;
  } 
  var state = JSON.parse(responseText);
  gobj("brightness").value = state.brightness;
  gobj("speed").value = state.speed;
  gobj("change").value = state.change;
  var collection = document.getElementsByClassName("mode");
  for(var i=0;i<collection.length;i++)
  {
    collection[i].checked = collection[i].id.slice(4) == state.mode;
  }
}
</script>
</body>
</html>
)rawliteral";
