entities = {
    [0] = {
        isEnemy = false,
        enforceXBoundary = true,
        enforceYBoundary = true,
        components = {
            position = {
                x = SCREEN_WIDTH / 2,
                y = SCREEN_HEIGHT - 16
            },
            draw = {
                textureAtlasLocation = "ship-player"
            },
            controllable = {
                xSpeed = 15.0,
                ySpeed = 15.0
            },
            collider = {
                xOffset = 0.0,
                yOffset = 0.0,
                width = 32,
                height = 32,
                damage = 1
            },
            health = {
                initialHealth = 10
            },
            leader = {
                followers = {
                    [0] = {
                        components = {
                            follower = {
                                xOffset = 16.0,
                                yOffset = 0.0
                            },
                            draw = {
                                textureAtlasLocation = "projectile-blue"
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.02,
                                velocity = 100.0,
                                angle = -500.0,
                                textureAtlasLocation = "projectile-blue",
                                forPlayer = true
                            }
                        }
                    },
                    [1] = {
                        components = {
                            follower = {
                                xOffset = -16.0,
                                yOffset = 0.0
                            },
                            draw = {
                                textureAtlasLocation = "projectile-blue"
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.02,
                                velocity = 100.0,
                                angle = -500.0,
                                textureAtlasLocation = "projectile-blue",
                                forPlayer = true
                            }
                        }
                    }
                }
            }
        }
    },
    [1] = {
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
                xOffset = 0.0,
                yOffset = 0.0,
                width = 32.0,
                height = 32.0,
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
    [2] = {
        isEnemy = true,
        enforceXBoundary = false,
        enforceYBoundary = false,
        components = {
            move = {
                xSpeed = 0.0,
                ySpeed = 75.0,
            },
            position = {
                x = 250.0,
                y = 0.0,
            },
            draw = {
                textureAtlasLocation = "ship-enemy"
            },
            collider = {
                xOffset = 0.0,
                yOffset = 0.0,
                width = 32.0,
                height = 32.0,
                damage = 1
            },
            health = {
                initialHealth = 5
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
                                rateOfFire = 0.2,
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
    [3] = {
        isEnemy = true,
        enforceXBoundary = false,
        enforceYBoundary = false,
        components = {
            move = {
                xSpeed = 0.0,
                ySpeed = 75.0,
            },
            position = {
                x = 500.0,
                y = 0.0,
            },
            draw = {
                textureAtlasLocation = "ship-enemy"
            },
            collider = {
                xOffset = 0.0,
                yOffset = 0.0,
                width = 32.0,
                height = 32.0,
                damage = 1
            },
            health = {
                initialHealth = 5
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
                                rateOfFire = 0.5,
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
    [4] = {
        isEnemy = false,
        enforceXBoundary = false,
        enforceYBoundary = false,
        components = {
            position = {
                x = SCREEN_WIDTH / 2,
                y = SCREEN_HEIGHT / 2
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