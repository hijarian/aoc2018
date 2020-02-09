fs.readFile(
	'Downloads/AoC2018/input_01.txt',
	(err, data) => {
		console.log(
			String(data)
				.split('\n')
				.reduce(
					(accumulator, value) => { return accumulator + +value; },
					0
				)
		);
	}
);
