var VXWORLD = (function() {
	var self = {};

	var sWidth, sHeight;

	var isShiftDown=false;
	var scene = new THREE.Scene();
	var camera = new THREE.PerspectiveCamera( 75, window.innerWidth/window.innerHeight, 0.1, 5000 );
	var controls;
	var dirLight, hemiLight;
	var mixers = [];
	var renderer = new THREE.WebGLRenderer();

	var mouse = new THREE.Vector2();
	var raycaster = new THREE.Raycaster();

	var voxels = [];
	var objects = [];
	var rollOverMesh, rollOverMaterial;
	var cubeGeo, cubeMaterial;
	var plane;

	var sWidth = window.innerWidth;
	console.log(window.innerHeight);
	console.log(document.getElementById("headerElement").clientHeight);
	console.log(document.getElementById("footerElement").clientHeight);
	var sHeight = window.innerHeight - document.getElementById("headerElement").clientHeight - document.getElementById("footerElement").clientHeight;
	//document.getElementById("mainCanvas").domElement = renderer.domElement;
	document.getElementById("mainCanvasContainer").appendChild( renderer.domElement );

	var selectedColor = {
		r : 255,
		g : 255,
		b : 255,
		a : 1
	}

	rollOverGeo = new THREE.BoxGeometry( 1, 1, 1 );
	rollOverMaterial = new THREE.MeshBasicMaterial( { color: 0xff0000, opacity: 0.5, transparent: true } );
	rollOverMesh = new THREE.Mesh( rollOverGeo, rollOverMaterial );
	scene.add( rollOverMesh );

	cubeGeo = new THREE.BoxGeometry( 1, 1, 1 );
	cubeMaterial = new THREE.MeshPhongMaterial( { color: 0xffffff, specular: 0xffffff, shininess: 20, morphTargets: true, vertexColors: THREE.FaceColors, shading: THREE.FlatShading } );

	var geometry = new THREE.BoxGeometry( 1, 1, 1 );
	var material = new THREE.MeshPhongMaterial( { color: 0xffffff, specular: 0xffffff, shininess: 20, morphTargets: true, vertexColors: THREE.FaceColors, shading: THREE.FlatShading } );
	var cube = new THREE.Mesh( geometry, material );
	//scene.add( cube );

	// grid
	var size = 50, step = 1;
	var geometry = new THREE.Geometry();
	for ( var i = - size; i <= size; i += step ) {
		geometry.vertices.push( new THREE.Vector3( - size, 0, i ) );
		geometry.vertices.push( new THREE.Vector3(   size, 0, i ) );
		geometry.vertices.push( new THREE.Vector3( i, 0, - size ) );
		geometry.vertices.push( new THREE.Vector3( i, 0,   size ) );
	}
	var material = new THREE.LineBasicMaterial( { color: 0x000000, opacity: 0.2, transparent: true } );
	var line = new THREE.LineSegments( geometry, material );
	scene.add( line );

	// camera controls
	
	controls = new THREE.TrackballControls( camera );
	controls.rotateSpeed = 1.0;
	controls.zoomSpeed = 1.2;
	controls.panSpeed = 0.8;
	controls.noZoom = false;
	controls.noPan = false;
	controls.staticMoving = true;
	controls.dynamicDampingFactor = 0.3;
	
	camera.position.set( 5, 8, 13 );
	camera.lookAt( new THREE.Vector3() );
	//camera.position.z = 5;
	//cube.rotation.x += 0.4;

	scene.fog = new THREE.Fog( 0xffffff, 1, 5000 );
	scene.fog.color.setHSL( 0.6, 0, 1 );
	// LIGHTS
	hemiLight = new THREE.HemisphereLight( 0xffffff, 0xffffff, 0.6 );
	hemiLight.color.setHSL( 0.6, 1, 0.6 );
	hemiLight.groundColor.setHSL( 0.095, 1, 0.75 );
	hemiLight.position.set( 0, 500, 0 );
	scene.add( hemiLight );

	dirLight = new THREE.DirectionalLight( 0xffffff, 1 );
	dirLight.color.setHSL( 0.1, 1, 0.95 );
	dirLight.position.set( -1, 1.75, 1 );
	dirLight.position.multiplyScalar( 50 );
	scene.add( dirLight );
	dirLight.castShadow = true;
	dirLight.shadow.mapSize.width = 2048;
	dirLight.shadow.mapSize.height = 2048;

	var d = 50;
	dirLight.shadow.camera.left = -d;
	dirLight.shadow.camera.right = d;
	dirLight.shadow.camera.top = d;
	dirLight.shadow.camera.bottom = -d;
	dirLight.shadow.camera.far = 3500;
	dirLight.shadow.bias = -0.0001;

	// GROUND
	var groundGeo = new THREE.PlaneBufferGeometry( 10000, 10000 );
	var groundMat = new THREE.MeshPhongMaterial( { color: 0xffffff, specular: 0x050505 } );
	groundMat.color.setHSL( 0.095, 1, 0.75 );
	var ground = new THREE.Mesh( groundGeo, groundMat );
	ground.rotation.x = -Math.PI/2;
	ground.position.y = -33;
	scene.add( ground );
	ground.receiveShadow = true;

	// SKYDOME
	var vertexShader = document.getElementById( 'vertexShader' ).textContent;
	var fragmentShader = document.getElementById( 'fragmentShader' ).textContent;
	var uniforms = {
		topColor:    { value: new THREE.Color( 0x0077ff ) },
		bottomColor: { value: new THREE.Color( 0xffffff ) },
		offset:      { value: 33 },
		exponent:    { value: 0.6 }
	};
	uniforms.topColor.value.copy( hemiLight.color );
	scene.fog.color.copy( uniforms.bottomColor.value );
	var skyGeo = new THREE.SphereGeometry( 4000, 32, 15 );
	var skyMat = new THREE.ShaderMaterial( { vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms, side: THREE.BackSide } );
	var sky = new THREE.Mesh( skyGeo, skyMat );
	scene.add( sky );

	var planegeometry = new THREE.PlaneBufferGeometry( 1000, 1000 );
	planegeometry.rotateX( - Math.PI / 2 );
	plane = new THREE.Mesh( planegeometry, new THREE.MeshBasicMaterial( { visible: false } ) );
	scene.add( plane );
	objects.push( plane );

	
	renderer.setClearColor( scene.fog.color );
	renderer.setPixelRatio( window.devicePixelRatio );
	renderer.setSize( sWidth, sHeight );

	renderer.gammaInput = true;
	renderer.gammaOutput = true;
	renderer.shadowMap.enabled = true;
	renderer.shadowMap.renderReverseSided = false;

	var render = function () {
		
		requestAnimationFrame( render );

		controls.update();

		//cube.rotation.y += 0.016;

		renderer.render(scene, camera);
	};

	renderer.domElement.addEventListener( 'mousemove', onDocumentMouseMove, false );
	renderer.domElement.addEventListener( 'mouseup', onDocumentMouseDown, false );
	renderer.domElement.addEventListener( 'mouseup', onDocumentMouseUp, false );
	renderer.domElement.addEventListener( 'keydown', onDocumentKeyDown, false );
	renderer.domElement.addEventListener( 'keyup', onDocumentKeyUp, false );

	window.addEventListener( 'resize', onWindowResize, false );

	function onDocumentMouseMove(event) {
		event.preventDefault();
		mouse.set( ( event.clientX / window.innerWidth ) * 2 - 1, - ( event.clientY / window.innerHeight ) * 2 + 1 );
		raycaster.setFromCamera( mouse, camera );
		var intersects = raycaster.intersectObjects( objects );
		//console.log(intersects);
		if ( intersects.length > 0 ) {

			var intersect = intersects[ 0 ];
			rollOverMesh.position.copy( intersect.point ).add( intersect.face.normal );
			//rollOverMesh.position.addScalar( 0.5 );
			rollOverMesh.position.divideScalar( 1 ).floor().multiplyScalar( 1 );//.addScalar( 0.5 );
		}
		
	}

	function onDocumentMouseDown(event) {
		event.preventDefault();

		mouse.set( ( event.clientX / window.innerWidth ) * 2 - 1, - ( event.clientY / window.innerHeight ) * 2 + 1 );
		raycaster.setFromCamera( mouse, camera );

		var intersects = raycaster.intersectObjects( objects );

		if ( intersects.length > 0 ) {
			var intersect = intersects[ 0 ];
			// delete cube
			if ( isShiftDown ) {
				if ( intersect.object != plane ) {
					scene.remove( intersect.object );
					objects.splice( objects.indexOf( intersect.object ), 1 );
				}
			// create cube
			} else {
				console.log(cubeMaterial.color);
				var voxel = new THREE.Mesh( cubeGeo, cubeMaterial );
				voxel.position.copy( intersect.point ).add( intersect.face.normal );
				voxel.position.divideScalar( 1 ).floor().multiplyScalar( 1 );//.addScalar( 0.5 );
				scene.add( voxel );
				objects.push( voxel );
			}
			
		}

	}

	function onDocumentMouseUp(event) {

	}

	function onDocumentKeyDown( event ) {
		switch( event.keyCode ) {
			case 16: isShiftDown = true; break;
		}
	}

	function onDocumentKeyUp (event) {
		switch( event.keyCode ) {
			case 16: isShiftDown = false; break;
		}
	}

	function onWindowResize() {
		sWidth = window.innerWidth;
		sHeight = window.innerHeight - document.getElementById("headerElement").clientHeight - document.getElementById("footerElement").clientHeight;
		camera.aspect = sWidth / sHeight;
		camera.updateProjectionMatrix();
		renderer.setSize( sWidth, sHeight );
	}

	self.selectColor = function(color) {
		console.log("selecting color");
		selectedColor = {
			r : color.r,
			g : color.g,
			b : color.b
		}

		cubeMaterial = new THREE.MeshPhongMaterial( { color: new THREE.Color("rgb("+selectedColor.r+","+selectedColor.g+","+selectedColor.b+")" ) , specular: 0xffffff, shininess: 20, morphTargets: true, vertexColors: THREE.FaceColors, shading: THREE.FlatShading } );
	}

	self.smoothVoxels = function (iterations) {

	};

	function voxelSmooth (n, position) {
		if(n > 0)
		{
			
		}
	}

	render();


	return self;
});

var VXVoxel = (function(){
	var self = this;



	return self;
});