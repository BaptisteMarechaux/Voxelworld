var VXVoxel = function(position, voxelSize){
	var self = this;

	self.geometry = {};

	self.adjacence = [0, 0, 0, 0, 0, 0, //Faces
						0, 0, 0, 0, //Top adjacence
						 0, 0, 0, 0]; //Bottom Adjacence

	/*Adjacence

	// 0  Front
	// 1  Left
	// 2  Right
	// 3  Back
	// 4  Top
	// 5  Bottom
	// 6  Top Front
	// 7  Top Left
	// 8  Top Right
	// 9  Top Back
	// 10 Bottom Front
	// 11 Bottom Left
	// 12 Bottom Right
	// 13 Bottom Back

	*/

	self.color = new THREE.Color( 0xff0000 );

	self.size = voxelSize * 10;

	function init(pos) {
		var tempCubeGeo = new THREE.BoxGeometry( self.size, self.size, self.size );
		var tempCubeMaterial = new THREE.MeshPhongMaterial( { color: 0xffffff, specular: 0xffffff, shininess: 20, morphTargets: true, vertexColors: THREE.FaceColors, shading: THREE.FlatShading } );

		self.geometry = new THREE.Mesh( tempCubeGeo, tempCubeMaterial );
		self.geometry.position.copy(pos);
		self.geometry.name = "voxelObject";
	}

	self.setAdjacence = function() {

	};

	self.changeColor = function(selectedColor) {
		geometry.material = new THREE.MeshPhongMaterial( { color: new THREE.Color("rgb("+selectedColor.r+","+selectedColor.g+","+selectedColor.b+")" ) , specular: 0xffffff, shininess: 20, morphTargets: true, vertexColors: THREE.FaceColors, shading: THREE.FlatShading } );
	}

	console.log(position);
	init(position);

	return self;
};
