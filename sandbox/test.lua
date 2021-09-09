
k_up_force = 5.0
k_rotation_speed = 100.0
k_gravity = 10.0
is_alive = false;
points = 0;
vertical_velocity = 0

function BeginPlay()   
    
end

function ProcessInput(self, mouseState)

    if mouseState.left_button_pressed then
        vertical_velocity = k_up_force
    end

end

function Update(self, DeltaTime)
    vertical_velocity = (vertical_velocity - (DeltaTime * k_gravity))
    _Move(self, 0, vertical_velocity)
    
    if vertical_velocity < 0.0 then
        rotation = _GetCurrentRotation(self);
        rotation = math.min((rotation + (DeltaTime * k_rotation_speed)), 45.0)
        _SetRotation(self, rotation)
    else
        _SetRotation(self, -30.0)
    end
end