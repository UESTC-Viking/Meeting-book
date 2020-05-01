<?php
$info['username'] = "1dmin";
$info['password'] = "test";
$s = serialize($info);
echo $s . "<br>";
$A_index = -1;
for ($i = 0; $i < strlen($s); ++$i) {
	if ($s[$i] === "1") {
		$A_index = $i % 16;
	}

	if ($i % 16 === 0) {
		echo "<br>$s[$i]";
	} else {
		echo "$s[$i]";
	}

}
echo "<br>The index of 1 is" . $A_index;
