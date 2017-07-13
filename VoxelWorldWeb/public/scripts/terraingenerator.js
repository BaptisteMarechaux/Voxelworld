var terrainGenerator = function() {
	var self = {};
	var worldWidth = 200, worldDepth = 100,
	worldHalfWidth = worldWidth / 2, worldHalfDepth = worldDepth / 2,
	data = self.generateHeight( worldWidth, worldDepth );

	function createTerrain() {

	}

	self.generateHeight = function( width, height ) {
		var data = [], perlin = new ImprovedNoise();
		var size = width * height, quality = 2, z = Math.random() * 100;
		for ( var j = 0; j < 4; j ++ ) {
			if ( j === 0 ) for ( var i = 0; i < size; i ++ ) data[ i ] = 0;
			for ( var i = 0; i < size; i ++ ) {
				var x = i % width, y = ( i / width ) | 0;
				data[ i ] += perlin.noise( x / quality, y / quality, z ) * quality;
			}
			quality *= 4;
		}
		return data;
	}

	return self;
};
