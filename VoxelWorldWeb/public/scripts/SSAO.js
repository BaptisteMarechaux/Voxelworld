
if ( ! Detector.webgl ) Detector.addGetWebGLMessage();
    var container, stats;
    var camera, scene, renderer;
    var depthMaterial, effectComposer, depthRenderTarget;
    var ssaoPass;
    var group;
    var depthScale = 1.0;
    var postprocessing = { enabled : true, renderMode: 0 }; // renderMode: 0('framebuffer'), 1('onlyAO')
	init();
	animate();
function init() {
	container = document.createElement( 'div' );
	document.body.appendChild( container );
	renderer = new THREE.WebGLRenderer( { antialias: false } );
	renderer.setClearColor( 0xa0a0a0 );
	renderer.setPixelRatio( window.devicePixelRatio );
	renderer.setSize( window.innerWidth, window.innerHeight );
	document.body.appendChild( renderer.domElement );
	camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 100, 700 );
	camera.position.z = 500;
        scene = new THREE.Scene();
	group = new THREE.Object3D();
	scene.add( group );
	var geometry = new THREE.IcosahedronGeometry( 5, 1 );
	for ( var i = 0; i < 200; i ++ ) {
		var material = new THREE.MeshBasicMaterial();
		material.color.r = Math.random();
		material.color.g = Math.random();
		material.color.b = Math.random();
		var mesh = new THREE.Mesh( geometry, material );
		mesh.position.x = Math.random() * 400 - 200;
		mesh.position.y = Math.random() * 400 - 200;
		mesh.position.z = Math.random() * 400 - 200;
		mesh.rotation.x = Math.random();
		mesh.rotation.y = Math.random();
		mesh.rotation.z = Math.random();
		mesh.scale.x = mesh.scale.y = mesh.scale.z = Math.random() * 10 + 1;
		group.add( mesh );
	}
	stats = new Stats();
	container.appendChild( stats.dom );
	// Init postprocessing
	initPostprocessing();
	// Init gui
	var gui = new dat.GUI();
	gui.add( postprocessing, "enabled" ).onChange();
	gui.add( postprocessing, "renderMode", { framebuffer: 0, onlyAO: 1 } ).onChange( renderModeChange ).listen();
	window.addEventListener( 'resize', onWindowResize, false );
}
function renderModeChange( value ) {
	if ( value == 0 ) {
		// framebuffer
		ssaoPass.uniforms[ 'onlyAO' ].value = false;
	} else if ( value == 1 ) {
		// onlyAO
		ssaoPass.uniforms[ 'onlyAO' ].value = true;
	} else {
		console.error( "Not define renderModeChange type: " + value );
	}
}
function onWindowResize() {
	var width = window.innerWidth;
	var height = window.innerHeight;
	camera.aspect = width / height;
	camera.updateProjectionMatrix();
	renderer.setSize( width, height );
	// Resize renderTargets
	ssaoPass.uniforms[ 'size' ].value.set( width, height );
	var pixelRatio = renderer.getPixelRatio();
	var newWidth  = Math.floor( width / pixelRatio ) || 1;
	var newHeight = Math.floor( height / pixelRatio ) || 1;
	depthRenderTarget.setSize( newWidth, newHeight );
	effectComposer.setSize( newWidth, newHeight );
}
function initPostprocessing() {
	// Setup render pass
	var renderPass = new THREE.RenderPass( scene, camera );
	// Setup depth pass
	depthMaterial = new THREE.MeshDepthMaterial();
	depthMaterial.depthPacking = THREE.RGBADepthPacking;
	depthMaterial.blending = THREE.NoBlending;
	var pars = { minFilter: THREE.LinearFilter, magFilter: THREE.LinearFilter };
	depthRenderTarget = new THREE.WebGLRenderTarget( window.innerWidth, window.innerHeight, pars );
	depthRenderTarget.texture.name = "SSAOShader.rt";
	// Setup SSAO pass
	ssaoPass = new THREE.ShaderPass( THREE.SSAOShader );
        ssaoPass.renderToScreen = true;
	//ssaoPass.uniforms[ "tDiffuse" ].value will be set by ShaderPass
	ssaoPass.uniforms[ "tDepth" ].value = depthRenderTarget.texture;
	ssaoPass.uniforms[ 'size' ].value.set( window.innerWidth, window.innerHeight );
	ssaoPass.uniforms[ 'cameraNear' ].value = camera.near;
	ssaoPass.uniforms[ 'cameraFar' ].value = camera.far;
	ssaoPass.uniforms[ 'onlyAO' ].value = ( postprocessing.renderMode == 1 );
	ssaoPass.uniforms[ 'aoClamp' ].value = 0.3;
	ssaoPass.uniforms[ 'lumInfluence' ].value = 0.5;
	// Add pass to effect composer
	effectComposer = new THREE.EffectComposer( renderer );
	effectComposer.addPass( renderPass );
	effectComposer.addPass( ssaoPass );
}
function animate() {
	requestAnimationFrame( animate );
	stats.begin();
	render();
	stats.end();
}
function render() {
	var timer = performance.now();
	group.rotation.x = timer * 0.0002;
	group.rotation.y = timer * 0.0001;
	if ( postprocessing.enabled ) {
		// Render depth into depthRenderTarget
		scene.overrideMaterial = depthMaterial;
		renderer.render( scene, camera, depthRenderTarget, true );
		// Render renderPass and SSAO shaderPass
		scene.overrideMaterial = null;
		effectComposer.render();
	} else {
		renderer.render( scene, camera );
	}
}
