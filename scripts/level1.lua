bulletPatterns = {
    [0] = {
        [0] = {
            velocity = 100.0,
            angle = -90.0,
            xOffset = 16.0,
            yOffset = 0.0,
            textureAtlasLocation = "projectile-blue"
        },
        [1] = {
            velocity = 100.0,
            angle = -90.0,
            xOffset = -16.0,
            yOffset = 0.0,
            textureAtlasLocation = "projectile-blue"
        }
    },
    [1] = {
        [0] = {
            velocity = 200.0,
            angle = 100.0,
            xOffset = 0.0,
            yOffset = 0.0,
            textureAtlasLocation = "projectile-red"
        },
        [1] = {
            velocity = 200.0,
            angle = -100.0,
            xOffset = 0.0,
            yOffset = 0.0,
            textureAtlasLocation = "projectile-red"
        }
    }
}
droppables = {
    [0] = {
        textureAtlasLocation = "specialbrew",
        points = 5,
        xSpeed = 20.0,
        ySpeed = 0.0
    }
}
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
                textureAtlasLocation = "ship-player",
                rotation = 0.0
            },
            controllable = {
                xSpeed = 15.0,
                ySpeed = 15.0
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
            attractor = {
                radius = 200.0,
                speed = 500.0
            },
            leader = {
                followers = {
                    [0] = {
                        components = {
                            follower = {
                                xOffset = 0.0,
                                yOffset = 0.0
                            },
                            draw = {
                                textureAtlasLocation = "projectile-blue",
                                rotation = 0.0
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.02,
                                angle = 0.0,
                                forPlayer = true,
                                bulletPattern = 0
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
            direction = {
                angle = 90.0,
                velocity = 50.0
            },
            position = {
                x = 1000.0,
                y = 0.0,
            },
            draw = {
                textureAtlasLocation = "ship-enemy",
                rotation = 0.0
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
            dropItem = {
                name = "specialbrew"
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
                                textureAtlasLocation = "projectile-red",
                                rotation = 0.0
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.1,
                                angle = 0.0,
                                forPlayer = false,
                                bulletPattern = 1
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
            direction = {
                angle = 90.0,
                velocity = 50.0
            },
            position = {
                x = 250.0,
                y = 0.0,
            },
            draw = {
                textureAtlasLocation = "ship-enemy",
                rotation = 0.0
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
            dropItem = {
                name = "specialbrew"
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
                                textureAtlasLocation = "projectile-red",
                                rotation = 0.0
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.2,
                                angle = 0.0,
                                forPlayer = false,
                                bulletPattern = 1
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
            direction = {
                angle = 90.0,
                velocity = 50.0
            },
            position = {
                x = 500.0,
                y = 0.0,
            },
            draw = {
                textureAtlasLocation = "ship-enemy",
                rotation = 0.0
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
            dropItem = {
                name = "specialbrew"
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
                                textureAtlasLocation = "projectile-red",
                                rotation = 0.0
                            },
                            bulletSpawnPoint = {
                                rateOfFire = 0.5,
                                angle = 0.0,
                                forPlayer = false,
                                bulletPattern = 1
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