# Notification System - Low Level Design

## Problem Statement
Design a multi-channel notification system that supports:
- Multiple notification channels (Email, SMS, Push Notification)
- User notification preferences
- Observer pattern for event-driven notifications
- Strategy pattern for channel selection
- Priority levels (Low, Medium, High, Critical)

## Key Design Patterns Used
- **Observer** — Events trigger notifications to subscribed users
- **Strategy** — Different delivery channels
- **Decorator** — Add logging, retry, or filtering around notification sending
- **Factory** — Create appropriate channel instances

## Classes
- `NotificationService` — Central service, manages subscriptions and dispatching
- `NotificationChannel` — Interface (Email, SMS, Push)
- `User` — Has preferences for channels
- `Notification` — Message, priority, target user
- `Event` — Trigger that causes notifications
