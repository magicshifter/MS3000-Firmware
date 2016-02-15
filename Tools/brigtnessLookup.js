var bits = 12;
var n = Math.pow(2, bits);

timeRatio = 1/10;

function mapIntensityToPwm(intensity) {
	return Math.pow(intensity/(n-1), 6);
}

var maxPwm = mapIntensityToPwm(n-1);

console.log("byte pwmLookup[2*", n, "] = {");

for (var i = 0; i < n; i++) {
	var pwm = mapIntensityToPwm(i)/maxPwm;
	
	var shortPwm, longPwm;

	if (pwm <= timeRatio) {
		shortPwm = pwm/timeRatio;
		longPwm = 0;
	}
	else {
		shortPwm = 1;
		longPwm = pwm-timeRatio;
	}
		
	shortPwm = Math.round(255*shortPwm);
	longPwm = Math.round(255*longPwm);


	console.log(shortPwm + ", " + longPwm + ", //" +  i + "->" + pwm);

}

console.log("};");
