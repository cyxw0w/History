<!DOCTYPE html>
<meta charset = "utf-8">
</body>
<script launguage = 'javascript'>
function test(){
  alert("버튼을 클릭함.");
}
</script>
<form>
  <div style="text-align:center">
  <fieldset>
    <legend> 일정 정보 </legend>
    <table>
      <tr>
        <td><label for = "days">날짜</label> </td>
        <td><input id = "days" type = "text" /> </td>
      </tr>
      <tr>
          <td><label for = "msg">일정</label> </td>
          <td><textarea id = "msg"></textarea></td>
        </tr>
      </table>
    </fieldset>
    <br></br>
    <br></br>
      <fieldset>
        <input type = 'Button' value="추가" onclick="test()">
      <input type = "submit"/>
      <input type = "reset" />
    </fieldset>
  </div>
  </form>
</body>
