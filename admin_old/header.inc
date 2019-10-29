<?
	$conn  = mysql_connect("localhost", "rift", "rift123");
	$core_conn = mysql_connect("localhost", "rscore", "rift");

	$def_path = "/home/rift/code/";

	$trust = $_COOKIE[trust];

function bit_lookup($type, $bitvector, $core_conn)
{
	if($bitvector == 0)
		return "None";

	$rstring = "";
	$new_res = mysql_db_query("rift_core","select * from bit_lookup where type='$type' and ($bitvector & bit) order by bit asc", $core_conn);
	for($count=0; $row = mysql_fetch_array($new_res); $count++)
		$rstring .= "$row[name]<br>";

	return $rstring;
}

function fetch_data($category, $current, $core_conn)
{
	$get_res = mysql_db_query("rift_core","select * from d_lookup where category='$category'", $core_conn);

	for($count=0; $drow = mysql_fetch_array($get_res); $count++)
	{
		echo "<option value=$drow[value] ";
		if($drow[value] == $current)
			echo "selected";
		echo ">$drow[name]</option>";
	}
}

function define_lookup($category, $value, $return, $conn)
{
	$get_res = mysql_db_query("rift_core","SELECT * FROM d_lookup WHERE category='$category' AND value=$value", $conn);
	$row = mysql_fetch_array($get_res);
	return $row[$return];
}

?> 
<STYLE type="text/css">
TABLE.disp	{font : 10pt Verdana,Arial,Helvetica;}
TR.bold		{font-weight: bold;}
</STYLE>

<table width=100% height=100%>
<tr>
	<td width=120 bgcolor="CCCCCC" valign=top>
		<center><b>Help</b></center>
		<hr>
<?
	if($trust >= 52)
	{
		echo "<li><a href='../help/add.phtml'>Add</a></li>";
	}
?>
		<li><a href='../help/search.phtml'>Search</a></li>
		<br>
<?
	if($trust >= 58)
	{
		echo "<center><b>Core</b></center>";
		echo "<hr>";	
		
		echo "<li><a href='../class/index.phtml'>Classes</a></li>";
		echo "<li><a href='../race/index.phtml'>Races</a></li>";
		
		echo "<center><b>Utils</b></center>";
		echo "<hr>";
		
		echo "<li><a href='../utils/gen_col.phtml'>T/C (Auto)</a></li>"; 
		echo "<li><a href='../utils/update_areaid.phtml'>Update AreaID</a></li>";
		echo "<li><a href='../utils/update_roomid.phtml'>Update RoomID</a></li>";
	}
?>
		<br>
		<br>
		<li><a href='../logout.phtml'>Logout</a></li>
	</td>
	<td valign=top>
