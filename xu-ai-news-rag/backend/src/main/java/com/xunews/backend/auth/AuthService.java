package com.xunews.backend.auth;

import com.xunews.backend.auth.dto.LoginRequest;
import com.xunews.backend.auth.dto.RegisterRequest;
import com.xunews.backend.security.JwtService;
import com.xunews.backend.user.Role;
import com.xunews.backend.user.User;
import com.xunews.backend.user.UserRepository;
import lombok.RequiredArgsConstructor;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.Map;

@Service
@RequiredArgsConstructor
public class AuthService {

    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;
    private final AuthenticationManager authenticationManager;
    private final JwtService jwtService;

    public String register(RegisterRequest req) {
        if (userRepository.existsByUsername(req.getUsername())) {
            throw new RuntimeException("Username already exists");
        }
        User user = User.builder()
                .username(req.getUsername())
                .email(req.getEmail())
                .passwordHash(passwordEncoder.encode(req.getPassword()))
                .role(Role.USER)
                .build();
        userRepository.save(user);
        return jwtService.generateToken(user.getUsername(), Map.of("role", user.getRole().name()));
    }

    public String login(LoginRequest req) {
        authenticationManager.authenticate(new UsernamePasswordAuthenticationToken(
                req.getUsername(), req.getPassword()
        ));
        return jwtService.generateToken(req.getUsername(), Map.of());
    }
}
