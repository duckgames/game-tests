enemies = {
    [0] = {
        isEnemy = true,
        enforceXBoundary = false,
        enforceYBoundary = false,
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
                xOffset = 16.0,
                yOffset = 16.0,
                width = 16.0,
                height = 16.0,
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
    },
    [1] = {
        isEnemy = false,
        enforceXBoundary = false,
        enforceYBoundary = false,
        components = {
            position = {
                x = 100.0,
                y = 100.0,
            },
            animation = {
                numFrames = 2,
                frameDuration = 0.2,
                loop = true,
                startFrame = 0,
                frames = {
                    [0] = {
                        frame = "legs_middle"
                    },
                    [1] = {
                        frame = "legs_run"
                    }
                }
            }
        }
    }
}