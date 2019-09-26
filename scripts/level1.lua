enemies = {
    [0] = {
        canCollideWithPlayer = true,
        components = {
            move = {
                xSpeed = 50.0,
                ySpeed = 50.0,
            },
            position = {
                x = 1000.0,
                y = 0.0,
            },
            draw = {
                textureAtlasLocation = "ship-enemy"
            },
            collider = {
                x = 1000.0,
                y = 0.0,
                width = 1.0,
                height = 1.0,
                damage = 1
            },
            health = {
                initialHealth = 10
            },
            score = {
                points = 5
            },
            leader = {
                followers = {
                    [0] = {
                        components = {
                            follower = {
                                xOffset = 0.0,
                                yOffset = 16.0
                            },
                            position = {
                                x = 1000.0,
                                y = 0.0
                            },
                            draw = {
                                textureAtlasLocation = "projectile-red"
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.1,
                                velocity = 200.0,
                                angle = 100.0,
                                textureAtlasLocation = "projectile-red",
                                forPlayer = false
                            }
                        }
                    }
                }
            }
        }
    }
}